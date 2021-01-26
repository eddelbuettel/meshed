#include "covariance_lmc.h"

using namespace std;

arma::mat CmaternInv(const arma::mat& x,
                     const double& sigmasq,
                     const double& phi, const double& nu, 
                     const double& tausq){

  double pow2_nu1_gammanu = pow(2.0, 1.0-nu) / R::gammafn(nu);
  arma::mat res = arma::zeros(x.n_rows, x.n_rows);
  for(int i = 0; i < x.n_rows; i++){
    arma::rowvec cri = x.row(i);
    for(int j = i; j < x.n_rows; j++){
      arma::rowvec delta = cri - x.row(j);
      double hphi = arma::norm(delta) * phi;
      if(hphi > 0.0){
        res(i, j) = sigmasq * pow(hphi, nu) * pow2_nu1_gammanu *
          R::bessel_k(hphi, nu, 1.0);
      } else {
        res(i, j) = sigmasq + tausq;
      }
    }
  }
  res = arma::symmatu(res);
  return res;
}


// matern
void matern_internal_inplace(arma::mat& res, 
                             const arma::mat& coords,
                             const arma::uvec& ix, const arma::uvec& iy, 
                             const double& phi, const double& nu, 
                          const double& sigmasq, const double& reparam,
                 double * bessel_ws, const double& nugginside=1e-7,  bool same=false){
  
  double sigmasq_reparam = sigmasq / reparam;
  
  int threadid;
#ifdef _OPENMP
  threadid = omp_get_thread_num();
#endif
  
  int bessel_ws_inc = 3; // nu+1 // increase?
  double pow2_nu1_gammanu_sigmasq_reparam = sigmasq_reparam * pow(2.0, 1.0-nu) / R::gammafn(nu);
  
  if(same){
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i));//x.row(i);
      for(int j=i; j<iy.n_rows; j++){
        arma::rowvec delta = cri - coords.row(iy(j));//y.row(j);
        double hphi = arma::norm(delta) * phi;
        if(hphi > 0.0){
          res(i, j) = pow(hphi, nu) * pow2_nu1_gammanu_sigmasq_reparam *
            R::bessel_k_ex(hphi, nu, 1.0, &bessel_ws[threadid*bessel_ws_inc]);
        } else {
          res(i, j) = sigmasq_reparam * (1.0 + nugginside);
        }
      }
    }
    res = arma::symmatu(res);
  } else {
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i));//x.row(i);
      for(int j=0; j<iy.n_rows; j++){
        arma::rowvec delta = cri - coords.row(iy(j));//y.row(j);
        double hphi = arma::norm(delta) * phi;
        if(hphi > 0.0){
          res(i, j) = pow(hphi, nu) * pow2_nu1_gammanu_sigmasq_reparam *
            R::bessel_k_ex(hphi, nu, 1.0, &bessel_ws[threadid*bessel_ws_inc]);
        } else {
          res(i, j) = sigmasq_reparam * (1.0 + nugginside);
        }
      }
    }
  }
  //return res;
}

// matern covariance with nu = p + 1/2, and p=0,1,2
void matern_halfint_inplace(arma::mat& res, 
                            const arma::mat& coords,
                            //const arma::mat& x, const arma::mat& y, 
                            const arma::uvec& ix, const arma::uvec& iy,
                            const double& phi, 
                            const double& sigmasq, const double& reparam,
                            bool same, int twonu){
  // 0 based indexing
  double sigmasq_reparam = sigmasq/reparam;
  
  if(same){
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i)); //x.row(i);
      for(int j=i; j<iy.n_rows; j++){
        arma::rowvec delta = cri - coords.row(iy(j)); //y.row(j);
        double hphi = arma::norm(delta) * phi;
        if(hphi > 0.0){
          if(twonu == 1){
            res(i, j) = sigmasq_reparam * exp(-hphi);
          } else {
            if(twonu == 3){
              res(i, j) = sigmasq_reparam * exp(-hphi) * (1 + hphi);
            } else {
              if(twonu == 5){
                res(i, j) = sigmasq_reparam * (1 + hphi + hphi*hphi / 3.0) * exp(-hphi);
              }
            }
          }
        } else {
          res(i, j) = sigmasq_reparam;
        }
      }
    }
    res = arma::symmatu(res);
  } else {
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i)); //x.row(i);
      for(int j=0; j<iy.n_rows; j++){
        arma::rowvec delta = cri - coords.row(iy(j)); //y.row(j);
        double hphi = arma::norm(delta) * phi;
        if(hphi > 0.0){
          if(twonu == 1){
            res(i, j) = sigmasq_reparam * exp(-hphi);
          } else {
            if(twonu == 3){
              res(i, j) = sigmasq_reparam * exp(-hphi) * (1 + hphi);
            } else {
              if(twonu == 5){
                res(i, j) = sigmasq_reparam * (1 + hphi + hphi*hphi / 3.0) * exp(-hphi);
              }
            }
          }
        } else {
          res(i, j) = sigmasq_reparam;
        }
      }
    }
  }
  //return res;
}


// powered exponential nu<2
void powerexp_inplace(arma::mat& res, 
                      const arma::mat& coords,
                      const arma::uvec& ix, const arma::uvec& iy, 
                      const double& phi, const double& nu, const double& sigmasq, const double& reparam,
                      bool same){

  double sigmasq_reparam = sigmasq / reparam;
  if(same){
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i));
      for(int j=i; j<iy.n_rows; j++){
        arma::rowvec delta = cri - coords.row(iy(j));
        double hnuphi = pow(arma::norm(delta), nu) * phi;
        if(hnuphi > 0.0){
          res(i, j) = exp(-hnuphi) * sigmasq_reparam;
        } else {
          res(i, j) = sigmasq_reparam;
        }
      }
    }
    res = arma::symmatu(res);
  } else {
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i));
      for(int j=0; j<iy.n_rows; j++){
        arma::rowvec delta = cri - coords.row(iy(j));
        double hnuphi = pow(arma::norm(delta), nu) * phi;
        if(hnuphi > 0.0){
          res(i, j) = exp(-hnuphi) * sigmasq_reparam;
        } else {
          res(i, j) = sigmasq_reparam;
        }
      }
    }
  }
}


// gneiting 2002 eq. 15 with a,c,beta left unknown
arma::mat gneiting2002(const arma::mat& coords,
                       const arma::uvec& ix, const arma::uvec& iy, 
                       const double& a, const double& c, const double& beta, bool same){
  // NOT reparametrized here
  arma::mat res = arma::zeros(ix.n_rows, iy.n_rows);
  arma::uvec timecol = arma::ones<arma::uvec>(1) * 2;
  if(same){
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i)).subvec(0, 1); //x.row(i);
      double ti = coords(ix(i), 2);
      for(int j=i; j<iy.n_rows; j++){
        double h = arma::norm(cri - coords.row(iy(j))); //y.row(j);
        double u = abs(coords(iy(j), 2) - ti);
        double umod = 1.0/(a * u + 1.0);
        res(i, j) = umod * exp(-c * h * pow(umod, beta/2.0) );
      }
    }
    res = arma::symmatu(res);
  } else {
    for(int i=0; i<ix.n_rows; i++){
      arma::rowvec cri = coords.row(ix(i)).subvec(0, 1); //x.row(i);
      double ti = coords(ix(i), 2);
      for(int j=0; j<iy.n_rows; j++){
        double h = arma::norm(cri - coords.row(iy(j))); //y.row(j);
        double u = abs(coords(iy(j), 2) - ti);
        double umod = 1.0/(a * u + 1.0);
        res(i, j) = umod * exp(-c * h * pow(umod, beta/2.0) );
      }
    }
  }
  return res;
}

arma::mat Correlationf(
    const arma::mat& coords,
    //const arma::mat& x, const arma::mat& y, 
    const arma::uvec& ix, const arma::uvec& iy,
    const arma::vec& theta,
    MaternParams& matern, bool same){
  // these are not actually correlation functions because they are reparametrized to have 
  // C(0) = 1/reparam
  arma::mat res = arma::zeros(ix.n_rows, iy.n_rows);
  
  if(coords.n_cols == 2){
    // spatial matern
    // reparametrized here
    if(!matern.estimating_nu){
      // exponential
      double phi = theta(0);
      double sigmasq = theta(1);
      
      int nutimes2 = matern.twonu;
      double reparam = 1.0; 
      if(matern.using_ps){
        reparam = pow(phi, .0 + nutimes2);
      }
      
      matern_halfint_inplace(res, coords, ix, iy, phi, sigmasq, reparam, same, nutimes2);
      
      return res;
    } else {
      double phi = theta(0);
      double nu = theta(1);
      double sigmasq = theta(2);
      
      double reparam = 1.0;
      if(matern.using_ps){
        // we divide by phi given the equivalence in 
        // zhang 2004, corrollary to Thm.2
        reparam = pow(phi, 2.0*nu);
      }
      
      double nugginside = 0;//1e-7;
      
      //powerexp_inplace(res, coords, ix, iy, phi, nu, sigmasq, reparam, same);
      matern_internal_inplace(res, coords, ix, iy, phi, nu, 
                  sigmasq, reparam, matern.bessel_ws, nugginside, same);
      
      return res;
    }
  } else {
    // theta 0: temporal decay, 
    // theta 1: spatial decay,
    // theta 2: separability
    return gneiting2002(coords, ix, iy, theta(0), theta(1), theta(2), same);
  }
}

void CviaKron_invsympd_(arma::cube& CCi, 
                        const arma::mat& coords, const arma::uvec& indx, 
                        int k, const arma::mat& theta, MaternParams& matern){
  for(int j=0; j<k; j++){
    CCi.slice(j) = arma::inv_sympd( Correlationf(coords, indx, indx, 
                                    theta.col(j), matern, true) );
  }
}

void inv_det_via_chol(arma::mat& xinv, double& ldet, const arma::mat& x){
  arma::mat xchol = arma::inv(arma::trimatl(arma::chol( arma::symmatu(x) , "lower")));
  ldet = arma::accu(log(xchol.diag()));
  xinv = xchol.t() * xchol;
}

void inv_det_via_qr(arma::mat& xinv, double& ldet, const arma::mat& x){
  arma::mat Q;
  arma::mat R;
  arma::qr(Q, R, x);
  
  xinv = arma::symmatu(arma::inv(arma::trimatu(R)) * Q.t());
  ldet = - 0.5 * arma::accu(log(abs(R.diag())));
}

double CviaKron_HRi_(arma::cube& H, arma::cube& Ri, arma::cube& Kppi, 
                     const arma::cube& Cxx,
                     const arma::mat& coords, 
                     const arma::uvec& indx, const arma::uvec& indy, 
                     int k, const arma::mat& theta, MaternParams& matern){
  
  double logdet=0;
  for(int j=0; j<k; j++){
    arma::mat Rinverted;
    if(indy.n_elem > 0){
      arma::mat Cxy = Correlationf(coords, indx, indy, 
                                   theta.col(j), matern, false);
      arma::mat Cyy_i = arma::inv_sympd( Correlationf(coords, indy, indy, 
                                                      theta.col(j), matern, true) );
      Kppi.slice(j) = Cyy_i;
      arma::mat Hloc = Cxy * Cyy_i;
      arma::mat Targmat = Cxx.slice(j) - Hloc * Cxy.t();
      
      // ? sometimes ill conditioned? -- condition number
      //if((matern.twonu > 1) || (theta.n_rows > 2)){
        //Targmat.diag() += 1e-10;
      //}
      // Rloc_ichol = arma::inv(arma::trimatl(arma::chol( arma::symmatu(Targmat) , "lower")));
      // logdet += arma::accu(log(Rloc_ichol.diag()));

      double temp_ldet = 0;
      //arma::mat Temp;
      //inv_det_via_qr(Rinverted, temp_ldet, Targmat);
      inv_det_via_chol(Rinverted, temp_ldet, Targmat);
      
      logdet += temp_ldet;
    
      H.slice(j) = Hloc;
    } else {
      arma::mat Targmat = Cxx.slice(j);
      double temp_ldet = 0;
      //inv_det_via_qr(Rinverted, temp_ldet, Targmat);
      inv_det_via_chol(Rinverted, temp_ldet, Targmat);
      logdet += temp_ldet;
    }
    Ri.slice(j) = Rinverted;//
  }
  return logdet;
}


void CviaKron_HRj_bdiag_(
    arma::cube& Hj, arma::cube& Rj, arma::cube& Rij,
    const arma::cube& Kxxi_cache,
    const arma::mat& coords, const arma::uvec& indx, 
    const arma::uvec& naix, const arma::uvec& indy, 
    int k, const arma::mat& theta, MaternParams& matern){
  // inplace version of CviaKron_HRj_bdiag

  arma::uvec oneuv = arma::ones<arma::uvec>(1);
  for(int j=0; j<k; j++){
    arma::mat Cyy_i = Kxxi_cache.slice(j);// 
    //arma::inv_sympd( Correlationf(coordsy, coordsy, theta.col(j), matern, true) );
    
    for(int ix=0; ix<indx.n_rows; ix++){
      if(naix(ix) == 1){
        arma::uvec indxi = oneuv*indx(ix);
        arma::mat Cxx = Correlationf(coords, indxi, indxi, 
                                     theta.col(j), matern, true);
        arma::mat Cxy = Correlationf(coords, indxi, indy,  
                                     theta.col(j), matern, false);
        arma::mat Hloc = Cxy * Cyy_i;
        arma::mat R = Cxx - Hloc * Cxy.t();
        
        Hj.subcube(j, 0, ix, j, Hj.n_cols-1, ix) = Hloc;
        Rj(j, j, ix) = R(0,0) < 0 ? 0.0 : R(0,0);
        Rij(j, j, ix) = R(0,0) < 1e-17 ? 0.0 : 1.0/R(0,0); //1e-10
      }
    }
  }
}


void CviaKron_HRj_chol_bdiag_wcache(
    arma::cube& Hj, arma::mat& Rjchol, 
    const arma::cube& Kxxi_cache, const arma::uvec& naix,
    const arma::mat& coords, const arma::uvec& indx, const arma::uvec& indy, 
    int k, const arma::mat& theta, MaternParams& matern){
  arma::uvec oneuv = arma::ones<arma::uvec>(1);
  for(int j=0; j<k; j++){
    arma::mat Cyy_i = Kxxi_cache.slice(j);//
    //arma::inv_sympd( Correlationf(coordsy, coordsy, theta.col(j), true) );
    
    for(int ix=0; ix<indx.n_rows; ix++){
      if(naix(ix) == 0){ // otherwise it's not missing
        arma::uvec indxi = oneuv * indx(ix);
        arma::mat Cxx = Correlationf(coords, indxi, indxi, 
                                     theta.col(j), matern, true);
        arma::mat Cxy = Correlationf(coords, indxi, indy,  
                                     theta.col(j), matern, false);
        arma::mat Hloc = Cxy * Cyy_i;
        
        Hj.slice(ix).row(j) = Hloc;
        double Rcholtemp = arma::conv_to<double>::from(
          Cxx - Hloc * Cxy.t() );
        Rcholtemp = Rcholtemp < 0 ? 0.0 : Rcholtemp;
        Rjchol(j,ix) = pow(Rcholtemp, .5); 
      }
    }
  }
}

void CviaKron_HRj_chol_bdiag(
    arma::cube& Hj, arma::mat& Rjchol, arma::cube& Kxxi,
    const arma::uvec& naix,
    const arma::mat& coords, const arma::uvec& indx, const arma::uvec& indy, 
    int k, const arma::mat& theta, MaternParams& matern){

  arma::uvec oneuv = arma::ones<arma::uvec>(1);
  Kxxi = arma::zeros(indy.n_elem, indy.n_elem, k);
  for(int j=0; j<k; j++){
    Kxxi.slice(j) = arma::inv_sympd( Correlationf(coords, indy, indy, 
                                     theta.col(j), matern, true) );
    for(int ix=0; ix<indx.n_rows; ix++){
      if(naix(ix) == 0){ // otherwise it's not missing
        arma::uvec indxi = oneuv * indx(ix);
        arma::mat Cxx = Correlationf(coords, indxi, indxi, 
                                     theta.col(j), matern, true);
        arma::mat Cxy = Correlationf(coords, indxi, indy,  
                                     theta.col(j), matern, false);
        arma::mat Hloc = Cxy * Kxxi.slice(j);
        
        Hj.slice(ix).row(j) = Hloc;
        double Rcholtemp = arma::conv_to<double>::from(
          Cxx - Hloc * Cxy.t() );
        Rcholtemp = Rcholtemp < 0 ? 0.0 : Rcholtemp;
        Rjchol(j, ix) = pow(Rcholtemp, .5); 
      }
    }
  }
}
