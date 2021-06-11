// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <RcppArmadillo.h>
#include <Rcpp.h>

using namespace Rcpp;

// Cov_matern
arma::mat Cov_matern(const arma::mat& x, const arma::mat& y, const double& sigmasq, const double& phi, const double& nu, const double& tausq, bool same, int nThreads);
RcppExport SEXP _meshed_Cov_matern(SEXP xSEXP, SEXP ySEXP, SEXP sigmasqSEXP, SEXP phiSEXP, SEXP nuSEXP, SEXP tausqSEXP, SEXP sameSEXP, SEXP nThreadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type x(xSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type y(ySEXP);
    Rcpp::traits::input_parameter< const double& >::type sigmasq(sigmasqSEXP);
    Rcpp::traits::input_parameter< const double& >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< const double& >::type nu(nuSEXP);
    Rcpp::traits::input_parameter< const double& >::type tausq(tausqSEXP);
    Rcpp::traits::input_parameter< bool >::type same(sameSEXP);
    Rcpp::traits::input_parameter< int >::type nThreads(nThreadsSEXP);
    rcpp_result_gen = Rcpp::wrap(Cov_matern(x, y, sigmasq, phi, nu, tausq, same, nThreads));
    return rcpp_result_gen;
END_RCPP
}
// Cov_matern2
arma::mat Cov_matern2(const arma::mat& x, const arma::mat& y, const double& phi, bool same, int twonu);
RcppExport SEXP _meshed_Cov_matern2(SEXP xSEXP, SEXP ySEXP, SEXP phiSEXP, SEXP sameSEXP, SEXP twonuSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type x(xSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type y(ySEXP);
    Rcpp::traits::input_parameter< const double& >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< bool >::type same(sameSEXP);
    Rcpp::traits::input_parameter< int >::type twonu(twonuSEXP);
    rcpp_result_gen = Rcpp::wrap(Cov_matern2(x, y, phi, same, twonu));
    return rcpp_result_gen;
END_RCPP
}
// Cov_matern_h
double Cov_matern_h(const double& h, const double& sigmasq, const double& phi, const double& nu, const double& tausq);
RcppExport SEXP _meshed_Cov_matern_h(SEXP hSEXP, SEXP sigmasqSEXP, SEXP phiSEXP, SEXP nuSEXP, SEXP tausqSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const double& >::type h(hSEXP);
    Rcpp::traits::input_parameter< const double& >::type sigmasq(sigmasqSEXP);
    Rcpp::traits::input_parameter< const double& >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< const double& >::type nu(nuSEXP);
    Rcpp::traits::input_parameter< const double& >::type tausq(tausqSEXP);
    rcpp_result_gen = Rcpp::wrap(Cov_matern_h(h, sigmasq, phi, nu, tausq));
    return rcpp_result_gen;
END_RCPP
}
// Cov_powexp_h
double Cov_powexp_h(const double& h, const double& sigmasq, const double& phi, const double& nu, const double& tausq);
RcppExport SEXP _meshed_Cov_powexp_h(SEXP hSEXP, SEXP sigmasqSEXP, SEXP phiSEXP, SEXP nuSEXP, SEXP tausqSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const double& >::type h(hSEXP);
    Rcpp::traits::input_parameter< const double& >::type sigmasq(sigmasqSEXP);
    Rcpp::traits::input_parameter< const double& >::type phi(phiSEXP);
    Rcpp::traits::input_parameter< const double& >::type nu(nuSEXP);
    Rcpp::traits::input_parameter< const double& >::type tausq(tausqSEXP);
    rcpp_result_gen = Rcpp::wrap(Cov_powexp_h(h, sigmasq, phi, nu, tausq));
    return rcpp_result_gen;
END_RCPP
}
// gneiting2002_h
double gneiting2002_h(const double& h, const double& u, const double& a, const double& c, const double& beta);
RcppExport SEXP _meshed_gneiting2002_h(SEXP hSEXP, SEXP uSEXP, SEXP aSEXP, SEXP cSEXP, SEXP betaSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const double& >::type h(hSEXP);
    Rcpp::traits::input_parameter< const double& >::type u(uSEXP);
    Rcpp::traits::input_parameter< const double& >::type a(aSEXP);
    Rcpp::traits::input_parameter< const double& >::type c(cSEXP);
    Rcpp::traits::input_parameter< const double& >::type beta(betaSEXP);
    rcpp_result_gen = Rcpp::wrap(gneiting2002_h(h, u, a, c, beta));
    return rcpp_result_gen;
END_RCPP
}
// blanket
arma::field<arma::uvec> blanket(const arma::field<arma::uvec>& parents, const arma::field<arma::uvec>& children, const arma::uvec& names, const arma::uvec& block_ct_obs);
RcppExport SEXP _meshed_blanket(SEXP parentsSEXP, SEXP childrenSEXP, SEXP namesSEXP, SEXP block_ct_obsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type parents(parentsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type children(childrenSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type names(namesSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type block_ct_obs(block_ct_obsSEXP);
    rcpp_result_gen = Rcpp::wrap(blanket(parents, children, names, block_ct_obs));
    return rcpp_result_gen;
END_RCPP
}
// coloring
arma::ivec coloring(const arma::field<arma::uvec>& blanket, const arma::uvec& block_names, const arma::uvec& block_ct_obs);
RcppExport SEXP _meshed_coloring(SEXP blanketSEXP, SEXP block_namesSEXP, SEXP block_ct_obsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type blanket(blanketSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type block_names(block_namesSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type block_ct_obs(block_ct_obsSEXP);
    rcpp_result_gen = Rcpp::wrap(coloring(blanket, block_names, block_ct_obs));
    return rcpp_result_gen;
END_RCPP
}
// kthresholdscp
arma::vec kthresholdscp(arma::vec x, int k);
RcppExport SEXP _meshed_kthresholdscp(SEXP xSEXP, SEXP kSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< arma::vec >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type k(kSEXP);
    rcpp_result_gen = Rcpp::wrap(kthresholdscp(x, k));
    return rcpp_result_gen;
END_RCPP
}
// part_axis_parallel
arma::mat part_axis_parallel(const arma::mat& coords, const arma::vec& Mv, int n_threads, bool verbose);
RcppExport SEXP _meshed_part_axis_parallel(SEXP coordsSEXP, SEXP MvSEXP, SEXP n_threadsSEXP, SEXP verboseSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type coords(coordsSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type Mv(MvSEXP);
    Rcpp::traits::input_parameter< int >::type n_threads(n_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    rcpp_result_gen = Rcpp::wrap(part_axis_parallel(coords, Mv, n_threads, verbose));
    return rcpp_result_gen;
END_RCPP
}
// part_axis_parallel_fixed
arma::mat part_axis_parallel_fixed(const arma::mat& coords, const arma::field<arma::vec>& thresholds, int n_threads);
RcppExport SEXP _meshed_part_axis_parallel_fixed(SEXP coordsSEXP, SEXP thresholdsSEXP, SEXP n_threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type coords(coordsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::vec>& >::type thresholds(thresholdsSEXP);
    Rcpp::traits::input_parameter< int >::type n_threads(n_threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(part_axis_parallel_fixed(coords, thresholds, n_threads));
    return rcpp_result_gen;
END_RCPP
}
// mesh_graph_cpp
Rcpp::List mesh_graph_cpp(const arma::mat& layers_descr, const arma::uvec& Mv, bool verbose, bool both_spatial_axes, int n_threads);
RcppExport SEXP _meshed_mesh_graph_cpp(SEXP layers_descrSEXP, SEXP MvSEXP, SEXP verboseSEXP, SEXP both_spatial_axesSEXP, SEXP n_threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type layers_descr(layers_descrSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type Mv(MvSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< bool >::type both_spatial_axes(both_spatial_axesSEXP);
    Rcpp::traits::input_parameter< int >::type n_threads(n_threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(mesh_graph_cpp(layers_descr, Mv, verbose, both_spatial_axes, n_threads));
    return rcpp_result_gen;
END_RCPP
}
// knn_naive
arma::umat knn_naive(const arma::mat& x, const arma::mat& search_here, const arma::vec& weights);
RcppExport SEXP _meshed_knn_naive(SEXP xSEXP, SEXP search_hereSEXP, SEXP weightsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type x(xSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type search_here(search_hereSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type weights(weightsSEXP);
    rcpp_result_gen = Rcpp::wrap(knn_naive(x, search_here, weights));
    return rcpp_result_gen;
END_RCPP
}
// mesh_graph_hyper
Rcpp::List mesh_graph_hyper(const arma::umat& bucbl, const arma::umat& bavail, const arma::vec& na_which, const arma::mat& centroids, const arma::mat& avcentroids, const arma::uvec& avblocks, int k);
RcppExport SEXP _meshed_mesh_graph_hyper(SEXP bucblSEXP, SEXP bavailSEXP, SEXP na_whichSEXP, SEXP centroidsSEXP, SEXP avcentroidsSEXP, SEXP avblocksSEXP, SEXP kSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::umat& >::type bucbl(bucblSEXP);
    Rcpp::traits::input_parameter< const arma::umat& >::type bavail(bavailSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type na_which(na_whichSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type centroids(centroidsSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type avcentroids(avcentroidsSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type avblocks(avblocksSEXP);
    Rcpp::traits::input_parameter< int >::type k(kSEXP);
    rcpp_result_gen = Rcpp::wrap(mesh_graph_hyper(bucbl, bavail, na_which, centroids, avcentroids, avblocks, k));
    return rcpp_result_gen;
END_RCPP
}
// meshed_casc
Rcpp::List meshed_casc(const arma::mat& y, const arma::uvec& family, const arma::mat& X, const arma::mat& coords, const arma::field<arma::uvec>& parents, const arma::field<arma::uvec>& children, const arma::vec& layer_names, const arma::vec& layer_gibbs_group, const arma::field<arma::uvec>& indexing, const arma::field<arma::uvec>& indexing_obs, const arma::mat& beta_Vi, int matern_twonu, const arma::mat& start_w, const arma::mat& lambda_values, const arma::umat& lambda_mask, const arma::mat& theta_values, const arma::mat& beta, const arma::vec& tausq_values, int maxit, int num_threads, bool adapting, bool use_cache, bool forced_grid, bool verbose, bool debug, int print_every, bool casc_beta, bool casc_w);
RcppExport SEXP _meshed_meshed_casc(SEXP ySEXP, SEXP familySEXP, SEXP XSEXP, SEXP coordsSEXP, SEXP parentsSEXP, SEXP childrenSEXP, SEXP layer_namesSEXP, SEXP layer_gibbs_groupSEXP, SEXP indexingSEXP, SEXP indexing_obsSEXP, SEXP beta_ViSEXP, SEXP matern_twonuSEXP, SEXP start_wSEXP, SEXP lambda_valuesSEXP, SEXP lambda_maskSEXP, SEXP theta_valuesSEXP, SEXP betaSEXP, SEXP tausq_valuesSEXP, SEXP maxitSEXP, SEXP num_threadsSEXP, SEXP adaptingSEXP, SEXP use_cacheSEXP, SEXP forced_gridSEXP, SEXP verboseSEXP, SEXP debugSEXP, SEXP print_everySEXP, SEXP casc_betaSEXP, SEXP casc_wSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type y(ySEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type family(familySEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type X(XSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type coords(coordsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type parents(parentsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type children(childrenSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type layer_names(layer_namesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type layer_gibbs_group(layer_gibbs_groupSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing(indexingSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing_obs(indexing_obsSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type beta_Vi(beta_ViSEXP);
    Rcpp::traits::input_parameter< int >::type matern_twonu(matern_twonuSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type start_w(start_wSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type lambda_values(lambda_valuesSEXP);
    Rcpp::traits::input_parameter< const arma::umat& >::type lambda_mask(lambda_maskSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type theta_values(theta_valuesSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type beta(betaSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type tausq_values(tausq_valuesSEXP);
    Rcpp::traits::input_parameter< int >::type maxit(maxitSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type adapting(adaptingSEXP);
    Rcpp::traits::input_parameter< bool >::type use_cache(use_cacheSEXP);
    Rcpp::traits::input_parameter< bool >::type forced_grid(forced_gridSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< bool >::type debug(debugSEXP);
    Rcpp::traits::input_parameter< int >::type print_every(print_everySEXP);
    Rcpp::traits::input_parameter< bool >::type casc_beta(casc_betaSEXP);
    Rcpp::traits::input_parameter< bool >::type casc_w(casc_wSEXP);
    rcpp_result_gen = Rcpp::wrap(meshed_casc(y, family, X, coords, parents, children, layer_names, layer_gibbs_group, indexing, indexing_obs, beta_Vi, matern_twonu, start_w, lambda_values, lambda_mask, theta_values, beta, tausq_values, maxit, num_threads, adapting, use_cache, forced_grid, verbose, debug, print_every, casc_beta, casc_w));
    return rcpp_result_gen;
END_RCPP
}
// meshed_mcmc
Rcpp::List meshed_mcmc(const arma::mat& y, const arma::uvec& family, const arma::mat& X, const arma::mat& coords, int k, const arma::field<arma::uvec>& parents, const arma::field<arma::uvec>& children, const arma::vec& layer_names, const arma::vec& layer_gibbs_group, const arma::field<arma::uvec>& indexing, const arma::field<arma::uvec>& indexing_obs, const arma::mat& set_unif_bounds_in, const arma::mat& beta_Vi, const arma::vec& sigmasq_ab, const arma::vec& tausq_ab, int matern_twonu, const arma::mat& start_w, const arma::mat& lambda, const arma::umat& lambda_mask, const arma::mat& theta, const arma::mat& beta, const arma::vec& tausq, const arma::mat& mcmcsd, int mcmc_keep, int mcmc_burn, int mcmc_thin, int mcmc_startfrom, int num_threads, bool adapting, bool use_cache, bool forced_grid, bool use_ps, bool verbose, bool debug, int print_every, bool sample_beta, bool sample_tausq, bool sample_lambda, bool sample_theta, bool sample_w);
RcppExport SEXP _meshed_meshed_mcmc(SEXP ySEXP, SEXP familySEXP, SEXP XSEXP, SEXP coordsSEXP, SEXP kSEXP, SEXP parentsSEXP, SEXP childrenSEXP, SEXP layer_namesSEXP, SEXP layer_gibbs_groupSEXP, SEXP indexingSEXP, SEXP indexing_obsSEXP, SEXP set_unif_bounds_inSEXP, SEXP beta_ViSEXP, SEXP sigmasq_abSEXP, SEXP tausq_abSEXP, SEXP matern_twonuSEXP, SEXP start_wSEXP, SEXP lambdaSEXP, SEXP lambda_maskSEXP, SEXP thetaSEXP, SEXP betaSEXP, SEXP tausqSEXP, SEXP mcmcsdSEXP, SEXP mcmc_keepSEXP, SEXP mcmc_burnSEXP, SEXP mcmc_thinSEXP, SEXP mcmc_startfromSEXP, SEXP num_threadsSEXP, SEXP adaptingSEXP, SEXP use_cacheSEXP, SEXP forced_gridSEXP, SEXP use_psSEXP, SEXP verboseSEXP, SEXP debugSEXP, SEXP print_everySEXP, SEXP sample_betaSEXP, SEXP sample_tausqSEXP, SEXP sample_lambdaSEXP, SEXP sample_thetaSEXP, SEXP sample_wSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type y(ySEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type family(familySEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type X(XSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type coords(coordsSEXP);
    Rcpp::traits::input_parameter< int >::type k(kSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type parents(parentsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type children(childrenSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type layer_names(layer_namesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type layer_gibbs_group(layer_gibbs_groupSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing(indexingSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing_obs(indexing_obsSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type set_unif_bounds_in(set_unif_bounds_inSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type beta_Vi(beta_ViSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type sigmasq_ab(sigmasq_abSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type tausq_ab(tausq_abSEXP);
    Rcpp::traits::input_parameter< int >::type matern_twonu(matern_twonuSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type start_w(start_wSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type lambda(lambdaSEXP);
    Rcpp::traits::input_parameter< const arma::umat& >::type lambda_mask(lambda_maskSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type theta(thetaSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type beta(betaSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type tausq(tausqSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type mcmcsd(mcmcsdSEXP);
    Rcpp::traits::input_parameter< int >::type mcmc_keep(mcmc_keepSEXP);
    Rcpp::traits::input_parameter< int >::type mcmc_burn(mcmc_burnSEXP);
    Rcpp::traits::input_parameter< int >::type mcmc_thin(mcmc_thinSEXP);
    Rcpp::traits::input_parameter< int >::type mcmc_startfrom(mcmc_startfromSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type adapting(adaptingSEXP);
    Rcpp::traits::input_parameter< bool >::type use_cache(use_cacheSEXP);
    Rcpp::traits::input_parameter< bool >::type forced_grid(forced_gridSEXP);
    Rcpp::traits::input_parameter< bool >::type use_ps(use_psSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< bool >::type debug(debugSEXP);
    Rcpp::traits::input_parameter< int >::type print_every(print_everySEXP);
    Rcpp::traits::input_parameter< bool >::type sample_beta(sample_betaSEXP);
    Rcpp::traits::input_parameter< bool >::type sample_tausq(sample_tausqSEXP);
    Rcpp::traits::input_parameter< bool >::type sample_lambda(sample_lambdaSEXP);
    Rcpp::traits::input_parameter< bool >::type sample_theta(sample_thetaSEXP);
    Rcpp::traits::input_parameter< bool >::type sample_w(sample_wSEXP);
    rcpp_result_gen = Rcpp::wrap(meshed_mcmc(y, family, X, coords, k, parents, children, layer_names, layer_gibbs_group, indexing, indexing_obs, set_unif_bounds_in, beta_Vi, sigmasq_ab, tausq_ab, matern_twonu, start_w, lambda, lambda_mask, theta, beta, tausq, mcmcsd, mcmc_keep, mcmc_burn, mcmc_thin, mcmc_startfrom, num_threads, adapting, use_cache, forced_grid, use_ps, verbose, debug, print_every, sample_beta, sample_tausq, sample_lambda, sample_theta, sample_w));
    return rcpp_result_gen;
END_RCPP
}
// spmeshed_predict
Rcpp::List spmeshed_predict(const arma::mat& predx, const arma::mat& predcoords, const arma::uvec& predblock, const arma::mat& coords, const arma::field<arma::uvec>& parents, const arma::uvec& block_names, const arma::field<arma::uvec>& indexing, const arma::field<arma::mat>& v_sampled, const arma::cube& theta_sampled, const arma::cube& lambda_sampled, const arma::cube& beta_sampled, const arma::mat& tausq_sampled, int twonu, bool use_ps, bool verbose, int num_threads);
RcppExport SEXP _meshed_spmeshed_predict(SEXP predxSEXP, SEXP predcoordsSEXP, SEXP predblockSEXP, SEXP coordsSEXP, SEXP parentsSEXP, SEXP block_namesSEXP, SEXP indexingSEXP, SEXP v_sampledSEXP, SEXP theta_sampledSEXP, SEXP lambda_sampledSEXP, SEXP beta_sampledSEXP, SEXP tausq_sampledSEXP, SEXP twonuSEXP, SEXP use_psSEXP, SEXP verboseSEXP, SEXP num_threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type predx(predxSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type predcoords(predcoordsSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type predblock(predblockSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type coords(coordsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type parents(parentsSEXP);
    Rcpp::traits::input_parameter< const arma::uvec& >::type block_names(block_namesSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing(indexingSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::mat>& >::type v_sampled(v_sampledSEXP);
    Rcpp::traits::input_parameter< const arma::cube& >::type theta_sampled(theta_sampledSEXP);
    Rcpp::traits::input_parameter< const arma::cube& >::type lambda_sampled(lambda_sampledSEXP);
    Rcpp::traits::input_parameter< const arma::cube& >::type beta_sampled(beta_sampledSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type tausq_sampled(tausq_sampledSEXP);
    Rcpp::traits::input_parameter< int >::type twonu(twonuSEXP);
    Rcpp::traits::input_parameter< bool >::type use_ps(use_psSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(spmeshed_predict(predx, predcoords, predblock, coords, parents, block_names, indexing, v_sampled, theta_sampled, lambda_sampled, beta_sampled, tausq_sampled, twonu, use_ps, verbose, num_threads));
    return rcpp_result_gen;
END_RCPP
}
// rmeshedgp_internal
arma::mat rmeshedgp_internal(const arma::mat& coords, const arma::field<arma::uvec>& parents, const arma::field<arma::uvec>& children, const arma::vec& layer_names, const arma::vec& layer_gibbs_group, const arma::field<arma::uvec>& indexing, const arma::field<arma::uvec>& indexing_obs, int matern_twonu, const arma::mat& theta, int num_threads, bool use_cache, bool verbose, bool debug);
RcppExport SEXP _meshed_rmeshedgp_internal(SEXP coordsSEXP, SEXP parentsSEXP, SEXP childrenSEXP, SEXP layer_namesSEXP, SEXP layer_gibbs_groupSEXP, SEXP indexingSEXP, SEXP indexing_obsSEXP, SEXP matern_twonuSEXP, SEXP thetaSEXP, SEXP num_threadsSEXP, SEXP use_cacheSEXP, SEXP verboseSEXP, SEXP debugSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::mat& >::type coords(coordsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type parents(parentsSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type children(childrenSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type layer_names(layer_namesSEXP);
    Rcpp::traits::input_parameter< const arma::vec& >::type layer_gibbs_group(layer_gibbs_groupSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing(indexingSEXP);
    Rcpp::traits::input_parameter< const arma::field<arma::uvec>& >::type indexing_obs(indexing_obsSEXP);
    Rcpp::traits::input_parameter< int >::type matern_twonu(matern_twonuSEXP);
    Rcpp::traits::input_parameter< const arma::mat& >::type theta(thetaSEXP);
    Rcpp::traits::input_parameter< int >::type num_threads(num_threadsSEXP);
    Rcpp::traits::input_parameter< bool >::type use_cache(use_cacheSEXP);
    Rcpp::traits::input_parameter< bool >::type verbose(verboseSEXP);
    Rcpp::traits::input_parameter< bool >::type debug(debugSEXP);
    rcpp_result_gen = Rcpp::wrap(rmeshedgp_internal(coords, parents, children, layer_names, layer_gibbs_group, indexing, indexing_obs, matern_twonu, theta, num_threads, use_cache, verbose, debug));
    return rcpp_result_gen;
END_RCPP
}
// cube_tcrossprod
arma::cube cube_tcrossprod(const arma::cube& x);
RcppExport SEXP _meshed_cube_tcrossprod(SEXP xSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::cube& >::type x(xSEXP);
    rcpp_result_gen = Rcpp::wrap(cube_tcrossprod(x));
    return rcpp_result_gen;
END_RCPP
}
// summary_list_mean
arma::mat summary_list_mean(const arma::field<arma::mat>& x, int n_threads);
RcppExport SEXP _meshed_summary_list_mean(SEXP xSEXP, SEXP n_threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::field<arma::mat>& >::type x(xSEXP);
    Rcpp::traits::input_parameter< int >::type n_threads(n_threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(summary_list_mean(x, n_threads));
    return rcpp_result_gen;
END_RCPP
}
// summary_list_q
arma::mat summary_list_q(const arma::field<arma::mat>& x, double q, int n_threads);
RcppExport SEXP _meshed_summary_list_q(SEXP xSEXP, SEXP qSEXP, SEXP n_threadsSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const arma::field<arma::mat>& >::type x(xSEXP);
    Rcpp::traits::input_parameter< double >::type q(qSEXP);
    Rcpp::traits::input_parameter< int >::type n_threads(n_threadsSEXP);
    rcpp_result_gen = Rcpp::wrap(summary_list_q(x, q, n_threads));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_meshed_Cov_matern", (DL_FUNC) &_meshed_Cov_matern, 8},
    {"_meshed_Cov_matern2", (DL_FUNC) &_meshed_Cov_matern2, 5},
    {"_meshed_Cov_matern_h", (DL_FUNC) &_meshed_Cov_matern_h, 5},
    {"_meshed_Cov_powexp_h", (DL_FUNC) &_meshed_Cov_powexp_h, 5},
    {"_meshed_gneiting2002_h", (DL_FUNC) &_meshed_gneiting2002_h, 5},
    {"_meshed_blanket", (DL_FUNC) &_meshed_blanket, 4},
    {"_meshed_coloring", (DL_FUNC) &_meshed_coloring, 3},
    {"_meshed_kthresholdscp", (DL_FUNC) &_meshed_kthresholdscp, 2},
    {"_meshed_part_axis_parallel", (DL_FUNC) &_meshed_part_axis_parallel, 4},
    {"_meshed_part_axis_parallel_fixed", (DL_FUNC) &_meshed_part_axis_parallel_fixed, 3},
    {"_meshed_mesh_graph_cpp", (DL_FUNC) &_meshed_mesh_graph_cpp, 5},
    {"_meshed_knn_naive", (DL_FUNC) &_meshed_knn_naive, 3},
    {"_meshed_mesh_graph_hyper", (DL_FUNC) &_meshed_mesh_graph_hyper, 7},
    {"_meshed_meshed_casc", (DL_FUNC) &_meshed_meshed_casc, 28},
    {"_meshed_meshed_mcmc", (DL_FUNC) &_meshed_meshed_mcmc, 40},
    {"_meshed_spmeshed_predict", (DL_FUNC) &_meshed_spmeshed_predict, 16},
    {"_meshed_rmeshedgp_internal", (DL_FUNC) &_meshed_rmeshedgp_internal, 13},
    {"_meshed_cube_tcrossprod", (DL_FUNC) &_meshed_cube_tcrossprod, 1},
    {"_meshed_summary_list_mean", (DL_FUNC) &_meshed_summary_list_mean, 2},
    {"_meshed_summary_list_q", (DL_FUNC) &_meshed_summary_list_q, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_meshed(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
