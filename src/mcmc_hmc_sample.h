#include <RcppArmadillo.h>
#include "R.h"
#include <numeric>

//#include "../distributions/mvnormal.h"

//#include "distparams.h"
#include "mcmc_hmc_nodes.h"
#include "mcmc_hmc_adapt.h"

inline arma::mat unvec(arma::vec q, int k){
  arma::mat result = arma::mat(q.memptr(), q.n_elem/k, k);
  return result;
}

// Position q and momentum p
struct pq_point {
  arma::vec q;
  arma::vec p;
  
  explicit pq_point(int n): q(n), p(n) {}
  pq_point(const pq_point& z): q(z.q.size()), p(z.p.size()) {
    q = z.q;
    p = z.p;
  }
  
  pq_point& operator= (const pq_point& z) {
    if (this == &z)
      return *this;
    
    q = z.q;
    p = z.p;
    
    return *this;
  }
};

template <class T>
inline void leapfrog(pq_point &z, float epsilon, T& postparams, int k=1){
  arma::mat qmat = unvec(z.q, k);
  z.p += epsilon * 0.5 * postparams.gradient_logfullcondit(qmat);
  //arma::vec qvecplus = arma::vectorise(z.q) + epsilon * z.p;
  z.q += epsilon * z.p;
  qmat = unvec(z.q, k);
  z.p += epsilon * 0.5 * postparams.gradient_logfullcondit(qmat);
}


template <class T>
inline double find_reasonable_stepsize(const arma::mat& current_q, T& postparams, const arma::mat& rnorm_mat){
  int K = current_q.n_elem;
  
  pq_point z(K);
  arma::vec p0 = //postparams.Michol * 
    arma::vectorise(rnorm_mat); //arma::randn(K);
  
  double epsilon = 1;
  
  arma::vec veccurq = arma::vectorise(current_q);
  
  z.q = veccurq;
  z.p = p0;
  
  double p_orig = postparams.logfullcondit(current_q) - 0.5* arma::conv_to<double>::from(z.p.t() * //postparams.M * 
                                                                                    z.p);//sum(z.p % z.p); 
  //Rcpp::Rcout << "before:  " << p_orig << "\n";
  leapfrog(z, epsilon, postparams, current_q.n_cols);
  //Rcpp::Rcout << "done leapfrog " << endl;
  arma::mat newq = unvec(z.q, current_q.n_cols);
  double p_prop = postparams.logfullcondit(newq) - 0.5* arma::conv_to<double>::from(z.p.t() * //postparams.M * 
                                                                                    z.p);//sum(z.p % z.p); 
  //Rcpp::Rcout << "after:  " << p_prop << "\n";
  double p_ratio = exp(p_prop - p_orig);
  double a = 2 * (p_ratio > .5) - 1;
  int it=0;
  bool condition = (pow(p_ratio, a) > pow(2.0, -a)) || std::isnan(p_ratio);
  
  while( condition & (it < 50) ){
    it ++;
    double twopowera = pow(2.0, a);
    epsilon = twopowera * epsilon;
    
    leapfrog(z, epsilon, postparams, current_q.n_cols);
    newq = unvec(z.q, current_q.n_cols);
    p_prop = postparams.logfullcondit(newq) - 0.5* arma::conv_to<double>::from(z.p.t() * //postparams.M * 
                                                                               z.p);//sum(z.p % z.p); 
    p_ratio = exp(p_prop - p_orig);
    
    condition = (pow(p_ratio, a)*twopowera > 1.0) || std::isnan(p_ratio);
    //Rcpp::Rcout << "epsilon : " << epsilon << " p_ratio " << p_ratio << " " << p_prop << "," << p_orig << " .. " << pow(p_ratio, a) << "\n";
    // reset
    z.q = veccurq;
    z.p = p0;
  }
  if(it == 50){
    epsilon = .01;
    //Rcpp::Rcout << "Set epsilon to " << epsilon << " after no reasonable stepsize could be found. (?)\n";
  }
  return epsilon/2.0;
} 

// mala and rm-mala

template <class T>
inline arma::mat sample_one_mala_cpp(arma::mat current_q, 
                                     T& postparams,
                                     AdaptE& adaptparams, 
                                     const arma::mat& rnorm_mat,
                                     const double& runifvar,
                                     bool rm=true, bool adapt=true,
                                     bool gibbs = false, bool debug=false){
  
  
  
  int k = current_q.n_cols;
  // currents
  arma::vec xgrad;
  double joint0, eps1, eps2;
  arma::mat MM, Minvchol, Minv;
  
  if(!rm){
    MM = arma::eye(current_q.n_elem, current_q.n_elem);
    xgrad = postparams.compute_dens_and_grad(joint0, current_q);
  } else {
    MM = postparams.compute_dens_grad_neghess(joint0, xgrad, current_q);
  }
  
  if(adapt & (!gibbs)){
    eps2 = pow(adaptparams.eps, 2.0);
    eps1 = adaptparams.eps;
  } else {
    eps2 = 2;// * adaptparams.eps;
    eps1 = 1;// * adaptparams.eps;
  }
  
  try {
    Minvchol = arma::inv(arma::trimatl(arma::chol(arma::symmatu(MM), "lower")));
  } catch(...) {
    MM = arma::eye(current_q.n_elem, current_q.n_elem);
    Minvchol = MM;
  }
  Minv = eps2 * 0.5 * Minvchol.t() * Minvchol;
  
  if(xgrad.has_inf() || std::isnan(joint0)){
    adaptparams.alpha = 0.0;
    adaptparams.n_alpha = 1.0;
    adaptparams.adapt_step();
    return current_q;
  }
  
  arma::vec veccurq = arma::vectorise(current_q);
  arma::vec proposal_mean = veccurq + Minv * xgrad;// / max(eps2, arma::norm(xgrad));
  
  // proposal value
  arma::vec p = arma::vectorise(rnorm_mat); //arma::randn(current_q.n_elem);  
  arma::vec q = proposal_mean + eps1 * Minvchol.t() * p;
  arma::mat qmat = arma::mat(q.memptr(), q.n_elem/k, k);

  // proposal
  double joint1; // = postparams.logfullcondit(qmat);
  arma::vec revgrad = postparams.compute_dens_and_grad(joint1, qmat);
  
  if(revgrad.has_inf() || std::isnan(joint1) || std::isinf(joint1)){
    adaptparams.alpha = 0.0;
    adaptparams.n_alpha = 1.0;
    adaptparams.adapt_step();
    return current_q;
  }
  
  arma::vec reverse_mean = q + Minv * revgrad;// / max(eps2, arma::norm(revgrad));;
  double prop0to1 = -.5/(eps1*eps1) * arma::conv_to<double>::from(
    (q - proposal_mean).t() * MM * (q - proposal_mean) );
  double prop1to0 = -.5/(eps1*eps1) * arma::conv_to<double>::from(
    (veccurq - reverse_mean).t() * MM * (veccurq - reverse_mean) );
  adaptparams.alpha = std::min(1.0, exp(joint1 + prop1to0 - joint0 - prop0to1));
  adaptparams.n_alpha = 1.0;
  if(runifvar < adaptparams.alpha){ 
    current_q = qmat;
  }
  adaptparams.adapt_step();
  return current_q;
}


// nuts

struct nuts_util {
  // Constants through each recursion
  double log_u; // uniform sample
  double H0; 	// Hamiltonian of starting point?
  int sign; 	// direction of the tree in a given iteration/recursion
  
  // Aggregators through each recursion
  int n_tree;
  double sum_prob; 
  bool criterion;
  
  // just to guarantee bool initializes to valid value
  nuts_util() : criterion(false) { }
};


inline bool compute_criterion(const arma::vec& p_sharp_minus, 
                              const arma::vec& p_sharp_plus,
                              const arma::vec& rho) {
  double crit1 = arma::conv_to<double>::from(p_sharp_plus.t() * rho);
  double crit2 = arma::conv_to<double>::from(p_sharp_minus.t() * rho);
  return crit1 > 0 && crit2 > 0;
}

template <class T>
inline int BuildTree(pq_point& z, pq_point& z_propose, 
                     arma::vec& p_sharp_left, 
                     arma::vec& p_sharp_right, 
                     arma::vec& rho, 
                     nuts_util& util, 
                     int depth, float epsilon,
                     T& postparams,
                     double& alpha,
                     double& n_alpha,
                     double joint_zero,
                       int k=1){
  
  //Rcpp::Rcout << "\n Tree direction:" << util.sign << " Depth:" << depth << std::endl;
  int K = z.q.n_rows;
  
  //std::default_random_engine generator;
  //std::uniform_real_distribution<double> unif01(0.0,1.0);
  //int F = postparams.W.n_rows;  
  float delta_max = 1000; // Recommended in the NUTS paper: 1000
  
  // Base case - take a single leapfrog step in the direction v
  if(depth == 0){
    leapfrog(z, epsilon, postparams, k);
    //leapfrog(z, util.sign * epsilon, postparams);
    
    arma::mat newq = unvec(z.q, k);
    float joint = postparams.logfullcondit(newq) - 0.5* arma::conv_to<double>::from(z.p.t() * //postparams.M * 
                                                                              z.p);//sum(z.p % z.p); 
    
    int valid_subtree = (util.log_u <= joint);    // Is the new point in the slice?
    util.criterion = util.log_u - joint < delta_max; // Is the simulation wildly inaccurate? // TODO: review
    util.n_tree += 1;
    
    //Rcpp::Rcout << "joint: " << joint << " joint_zero: " << joint_zero << "\n";
    alpha = std::min(1.0, exp( joint - joint_zero ));
    n_alpha = 1;
    
    z_propose = z;
    rho += z.p;
    p_sharp_left = z.p;  // p_sharp = inv(M)*p (Betancourt 58)
    p_sharp_right = p_sharp_left;
    
    return valid_subtree;
  } 
  
  // General recursion
  arma::vec p_sharp_dummy(K);
  
  // Build the left subtree
  arma::vec rho_left = arma::zeros(K);
  double alpha_prime1=0;
  double n_alpha_prime1=0;
  int n1 = BuildTree(z, z_propose, p_sharp_left, p_sharp_dummy, 
                     rho_left, util, depth-1, epsilon, postparams, 
                     alpha_prime1, n_alpha_prime1, joint_zero, k);
  
  if (!util.criterion) return 0; // early stopping
  
  // Build the right subtree
  pq_point z_propose_right(z);
  arma::vec rho_right(K); rho_left.zeros();
  double alpha_prime2=0;
  double n_alpha_prime2=0;
  int n2 = BuildTree(z, z_propose_right, p_sharp_dummy, p_sharp_right, 
                     rho_right, util, depth-1, epsilon, postparams, 
                     alpha_prime2, n_alpha_prime2, joint_zero, k);
  
  // Choose which subtree to propagate a sample up from.
  double accept_prob = static_cast<double>(n2) / std::max((n1 + n2), 1); // avoids 0/0;
  //Rcpp::RNGScope scope;
  float rand01 = R::runif(0, 1);//unif01(generator);
  if(util.criterion && (rand01 < accept_prob)){
    z_propose = z_propose_right;
  }
  
  // Break when NUTS criterion is no longer satisfied
  arma::vec rho_subtree = rho_left + rho_right;
  rho += rho_subtree;
  util.criterion = compute_criterion(p_sharp_left, p_sharp_right, rho);
  
  int n_valid_subtree = n1 + n2;
  
  alpha = alpha_prime1 + alpha_prime2;
  n_alpha = n_alpha_prime1 + n_alpha_prime2;
  
  return(n_valid_subtree);
}

template <class T>
inline arma::mat sample_one_nuts_cpp(arma::mat current_q, 
                                     T& postparams,
                                     AdaptE& adaptparams){
  
  
  int ksize = current_q.n_cols;
  int K = current_q.n_elem;
  //int F = W.n_rows;
  int MAXDEPTH = 7;
  
  //arma::mat h_n_samples(K, iter);   // traces of p
  arma::vec p0 = //postparams.Michol * 
    arma::randn(K);                  // initial momentum
  //current_q = log(current_q); 		// Transform to unrestricted space
  //h_n_samples.col(1) = current_q;
  
  pq_point z(K);
  
  nuts_util util;
  
  // Initialize the path. Proposed sample,
  // and leftmost/rightmost position and momentum
  ////////////////////////
  
  
  arma::vec veccurq = arma::vectorise(current_q);
  
  z.q = veccurq;
  
  z.p = p0;
  pq_point z_plus(z);
  pq_point z_minus(z);
  pq_point z_propose(z);
  
  // Utils o compute NUTS stop criterion
  arma::vec p_sharp_plus = z.p;
  arma::vec p_sharp_dummy = p_sharp_plus;
  arma::vec p_sharp_minus = p_sharp_plus;
  arma::vec rho(z.p);
  
  
  // Hamiltonian
  // Joint logprobability of position q and momentum p
  //Rcpp::Rcout << "sample_one_nuts_cpp: \n";
  double current_logpost = postparams.logfullcondit(current_q);
  ///Rcpp::Rcout << "starting from: " << current_logpost << "\n";
  double joint = current_logpost - 0.5* arma::conv_to<double>::from(z.p.t() * //postparams.M * 
                                                                    z.p);//sum(z.p % z.p); 
  
  // Slice variable
  ///////////////////////
  // Sample the slice variable: u ~ uniform([0, exp(joint)]). 
  // Equivalent to: (log(u) - joint) ~ exponential(1).
  // logu = joint - exprnd(1);
  //Rcpp::RNGScope scope;
  float random = R::rexp(1); //exp1(generator);
  util.log_u = joint - random;
  
  int n_valid = 1;
  util.criterion = true;
  
  // Build a trajectory until the NUTS criterion is no longer satisfied
  int depth_ = 0;
  //int divergent_ = 0;
  util.n_tree = 0;
  util.sum_prob = 0;
  
  
  // Build a balanced binary tree until the NUTS criterion fails
  while(util.criterion && (depth_ < MAXDEPTH)){
    //Rcpp::Rcout << "*****depth : " << depth_  << std::endl;
    
    // Build a new subtree in the chosen direction
    // (Modifies z_propose, z_minus, z_plus)
    arma::vec rho_subtree = arma::zeros(K);
    
    // Build a new subtree in a random direction
    //Rcpp::RNGScope scope;
    util.sign = 2 * (R::runif(0, 1) < 0.5) - 1;
    int n_valid_subtree=0;
    if(util.sign == 1){    
      z.pq_point::operator=(z_minus);
      n_valid_subtree = BuildTree(z, z_propose, p_sharp_dummy, p_sharp_plus, rho_subtree, 
                                  util, depth_, adaptparams.eps, postparams, 
                                  adaptparams.alpha, adaptparams.n_alpha, joint, ksize);
      z_plus.pq_point::operator=(z);
    } else {  
      z.pq_point::operator=(z_plus);
      n_valid_subtree = BuildTree(z, z_propose, p_sharp_dummy, p_sharp_minus, rho_subtree, 
                                  util, depth_, adaptparams.eps, postparams, 
                                  adaptparams.alpha, adaptparams.n_alpha, joint, ksize);
      z_minus.pq_point::operator=(z);
    }
    ++depth_;  // Increment depth.
    
    if(util.criterion){ 
      // Use Metropolis-Hastings to decide whether or not to move to a
      // point from the half-tree we just generated.
      double subtree_prob = std::min(1.0, static_cast<double>(n_valid_subtree)/n_valid);
      //Rcpp::RNGScope scope;
      if(R::runif(0, 1) < subtree_prob){ 
        
        arma::mat newq = unvec(z_propose.q, ksize);
        
        current_q = newq; // Accept proposal
      }
    }
    
    // Update number of valid points we've seen.
    n_valid += n_valid_subtree;
    
    // Break when NUTS criterion is no longer satisfied
    rho += rho_subtree;
    util.criterion = util.criterion && compute_criterion(p_sharp_minus, p_sharp_plus, rho);
  } // end while
  
  //Rcpp::Rcout << "eps: " << adaptparams.eps << "\n";
  //Rcpp::Rcout << "DEPTH: " << depth_ << "\n";
  
  // adapting
  adaptparams.adapt_step();
  
  //current_logpost = loglike_cpp(current_q, postparams);
  //Rcpp::Rcout << "ending with: " << current_logpost << "\n";
  
  return current_q;
}


template <class T>
inline arma::mat newton_step(arma::mat current_q, 
                            T& postparams,
                            AdaptE& adaptparams,
                            double eps=1,
                            bool debug=false){
  
  int k = current_q.n_cols;
  
  std::chrono::steady_clock::time_point t0;
  std::chrono::steady_clock::time_point t1;
  //double timer=0;
  
  // currents
  arma::vec xgrad;
  double joint0;
  arma::mat MM, Minvchol, Minv;
  
  MM = postparams.compute_dens_grad_neghess(joint0, xgrad, current_q);
  
  try {
    Minvchol = arma::inv(arma::trimatl(arma::chol(arma::symmatu(MM), "lower")));
  } catch(...) {
    MM = arma::eye(current_q.n_elem, current_q.n_elem);
    Minvchol = MM;
  }
  
  Minv = Minvchol.t() * Minvchol;
  
  if(xgrad.has_inf() || std::isnan(joint0) || xgrad.has_nan()){
    return current_q;
  }
  
  arma::vec veccurq = arma::vectorise(current_q);

  arma::vec q = veccurq + eps * Minv * xgrad;
  arma::mat qmat = arma::mat(q.memptr(), q.n_elem/k, k);
  
  return qmat;
}




