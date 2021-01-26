#include "meshed.h"

using namespace std;

void Meshed::deal_with_w(MeshDataLMC& data){
  
  Rcpp::RNGScope scope;
  rand_norm_mat = arma::randn(coords.n_rows, k);
  
  if(false & (gibbs_or_hmc == false)){
    gibbs_sample_w(data);
  } else {
    hmc_sample_w(data);
  }
}
  
void Meshed::update_block_w_cache(int u, MeshDataLMC& data){
  // 
  arma::mat Sigi_tot = build_block_diagonal_ptr(data.w_cond_prec_ptr.at(u));
  
  arma::mat Smu_tot = arma::zeros(k*indexing(u).n_elem, 1); // replace with fill(0)
  for(int c=0; c<children(u).n_elem; c++){
    int child = children(u)(c);
    arma::cube AK_u = cube_cols_ptr(data.w_cond_mean_K_ptr.at(child), u_is_which_col_f(u)(c)(0));
    AKuT_x_R_ptr(data.AK_uP(u)(c), AK_u, data.w_cond_prec_ptr.at(child)); 
    add_AK_AKu_multiply_(Sigi_tot, data.AK_uP(u)(c), AK_u);
  }
  
  if(forced_grid){
    int indxsize = indexing(u).n_elem;
    arma::mat yXB = arma::trans(y.rows(indexing_obs(u)) - XB.rows(indexing_obs(u)));
    for(int ix=0; ix<indexing_obs(u).n_elem; ix++){
      if(na_1_blocks(u)(ix) == 1){
        arma::mat LambdaH = arma::zeros(q, k*indxsize);
        for(int j=0; j<q; j++){
          if(na_mat(indexing_obs(u)(ix), j) == 1){
            arma::mat Hloc = data.Hproject(u).slice(ix);
            for(int jx=0; jx<k; jx++){
              arma::mat Hsub = Hloc.row(jx); //data.Hproject(u).subcube(jx,0,ix,jx,indxsize-1, ix);
              // this outcome margin observed at this location
              
              LambdaH.submat(j, jx*indxsize, j, (jx+1)*indxsize-1) += Lambda(j, jx) * Hsub;
            }
          }
        }
        arma::mat LambdaH_DplusSi = LambdaH.t() * data.DplusSi.slice(indexing_obs(u)(ix));
        Smu_tot += LambdaH_DplusSi * yXB.col(ix);
        Sigi_tot += LambdaH_DplusSi * LambdaH;
      }
    }
  } else {
    arma::mat u_tau_inv = arma::zeros(indexing_obs(u).n_elem, q);
    arma::mat ytilde = arma::zeros(indexing_obs(u).n_elem, q);
    
    for(int j=0; j<q; j++){
      for(int ix=0; ix<indexing_obs(u).n_elem; ix++){
        if(na_mat(indexing_obs(u)(ix), j) == 1){
          u_tau_inv(ix, j) = pow(tausq_inv(j), .5);
          ytilde(ix, j) = (y(indexing_obs(u)(ix), j) - XB(indexing_obs(u)(ix), j))*u_tau_inv(ix, j);
        }
      }
      // dont erase:
      //Sigi_tot += arma::kron( arma::trans(Lambda.row(j)) * Lambda.row(j), arma::diagmat(u_tau_inv%u_tau_inv));
      arma::mat LjtLj = arma::trans(Lambda.row(j)) * Lambda.row(j);
      arma::vec u_tausq_inv = u_tau_inv.col(j) % u_tau_inv.col(j);
      add_LtLxD(Sigi_tot, LjtLj, u_tausq_inv);
      
      Smu_tot += arma::vectorise(arma::diagmat(u_tau_inv.col(j)) * ytilde.col(j) * Lambda.row(j));
    }
  }
  data.Smu_start(u) = Smu_tot;
  data.Sigi_chol(u) = Sigi_tot;
  
  if((k>q) & (q>1)){
    arma::uvec blockdims = arma::cumsum( indexing(u).n_elem * arma::ones<arma::uvec>(k) );
    blockdims = arma::join_vert(oneuv * 0, blockdims);
    arma::uvec blockdims_q = blockdims.subvec(0, q-1);
    // WARNING: if k>q then we have only updated the lower block-triangular part of Sigi_tot for cholesky!
    // WARNING: we make use ONLY of the lower-blocktriangular part of Sigi_tot here.
    block_invcholesky_(data.Sigi_chol(u), blockdims_q);
  } else {
    data.Sigi_chol(u) = arma::inv(arma::trimatl(arma::chol( arma::symmatu( Sigi_tot ), "lower")));
    // try { 
    //   
    // } catch(...) {
    //   Sigi_tot.diag() += 1e-9; // poorly conditioned system
    //   data.Sigi_chol(u) = arma::inv(arma::trimatl(arma::chol( arma::symmatu( Sigi_tot ), "lower")));
    // }
    
  }
  
}

void Meshed::sample_nonreference_w(int u, MeshDataLMC& data, const arma::mat& rand_norm_mat){
  //message("[sample_nonreference_w] start.");
  // for updating lambda and tau which will only look at observed locations
  // centered updates instead use the partially marginalized thing
  
  for(int ix=0; ix<indexing_obs(u).n_elem; ix++){
    if(na_1_blocks(u)(ix) == 1){
      arma::mat Stemp = data.Riproject(u).slice(ix);
      arma::mat tsqi = tausq_inv;
      for(int j=0; j<q; j++){
        if(na_mat(indexing_obs(u)(ix), j) == 0){
          tsqi(j) = 0;
        }
      }
      
      arma::mat Smu_par = Stemp.diag() % arma::vectorise(w.row(indexing_obs(u)(ix)));
      arma::mat Smu_y = Lambda.t() * (tsqi % arma::trans(y.row(indexing_obs(u)(ix)) - XB.row(indexing_obs(u)(ix))));
      arma::mat Smu_tot = Smu_par + Smu_y;
      
      arma::mat Sigi_tot = Stemp + Lambda.t() * arma::diagmat(tsqi) * Lambda;
      
      arma::mat Sigi_chol;
      try {
        Sigi_chol = arma::inv(arma::trimatl(arma::chol( arma::symmatu( Sigi_tot ), "lower")));
      } catch(...) {
        Rcpp::Rcout << Sigi_tot << endl
                    << tsqi << endl
                    << Lambda << endl
                    << Stemp << endl;
        throw 1;
      }
      
      arma::vec rnvec = arma::vectorise(rand_norm_mat.row(indexing_obs(u)(ix)));
      arma::vec wmean = Sigi_chol.t() * Sigi_chol * Smu_tot;
      
      wU.row(indexing_obs(u)(ix)) = arma::trans(wmean + Sigi_chol.t() * rnvec);
    }
  }
  //message("[sample_nonreference_w] done.");
}

void Meshed::refresh_w_cache(MeshDataLMC& data){
  if(verbose & debug){
    Rcpp::Rcout << "[refresh_w_cache] \n";
  }
  start_overall = std::chrono::steady_clock::now();
  for(int i=0; i<n_blocks; i++){
    int u=block_names(i)-1;
    update_block_w_cache(u, data);
  }
  
  if(verbose & debug){
    end_overall = std::chrono::steady_clock::now();
    Rcpp::Rcout << "[refresh_w_cache] "
                << std::chrono::duration_cast<std::chrono::microseconds>(end_overall - start_overall).count()
                << "us. " << "\n";
  }
}

void Meshed::gibbs_sample_w(MeshDataLMC& data){
  if(verbose & debug){
    Rcpp::Rcout << "[gibbs_sample_w] " << "\n";
  }
  //Rcpp::Rcout << "Lambda from:  " << Lambda_orig(0, 0) << " to  " << Lambda(0, 0) << endl;
  
  
  start_overall = std::chrono::steady_clock::now();
  
  
  
  for(int g=0; g<n_gibbs_groups; g++){
#ifdef _OPENMP
    //***#pragma omp parallel for 
#endif
    for(int i=0; i<u_by_block_groups(g).n_elem; i++){
      int u = u_by_block_groups(g)(i);
      
      if((block_ct_obs(u) > 0)){
        
        update_block_w_cache(u, data);
        // recompute conditional mean
        arma::mat Smu_tot = data.Smu_start(u); //
        
        if(parents(u).n_elem>0){
          add_smu_parents_ptr_(Smu_tot, data.w_cond_prec_ptr.at(u), data.w_cond_mean_K_ptr.at(u),
                               w.rows( parents_indexing(u) ));
        } 
        
        for(int c=0; c<children(u).n_elem; c++){
          int child = children(u)(c);
          //---------------------
          arma::cube AK_u = cube_cols_ptr(data.w_cond_mean_K_ptr.at(child), u_is_which_col_f(u)(c)(0));
          
          arma::mat w_child = w.rows(indexing(child));
          arma::mat w_parchild = w.rows(parents_indexing(child));
          //---------------------
          if(parents(child).n_elem > 1){
            arma::cube AK_others = cube_cols_ptr(data.w_cond_mean_K_ptr.at(child), u_is_which_col_f(u)(c)(1));
            
            arma::mat w_parchild_others = w_parchild.rows(u_is_which_col_f(u)(c)(1));
            Smu_tot += 
              arma::vectorise(AK_vec_multiply(data.AK_uP(u)(c), 
                                              w_child - AK_vec_multiply(AK_others, w_parchild_others)));
          } else {
            Smu_tot += 
              arma::vectorise(AK_vec_multiply(data.AK_uP(u)(c), w_child));
          }
        }
        
        // sample
        arma::vec rnvec = arma::vectorise(rand_norm_mat.rows(indexing(u)));
        arma::vec wmean = data.Sigi_chol(u).t() * data.Sigi_chol(u) * Smu_tot;
        arma::vec wtemp = wmean + data.Sigi_chol(u).t() * rnvec;
        
        w.rows(indexing(u)) = 
          arma::mat(wtemp.memptr(), wtemp.n_elem/k, k); 
        
        // non-ref effect on y at all locations. here we have already sampled
        wU.rows(indexing(u)) = w.rows(indexing(u));
        
        if(forced_grid){
          for(int ix=0; ix<indexing_obs(u).n_elem; ix++){
            if(na_1_blocks(u)(ix) == 1){
              arma::mat wtemp = arma::sum(arma::trans(data.Hproject(u).slice(ix) % arma::trans(w.rows(indexing(u)))), 0);
              
              w.row(indexing_obs(u)(ix)) = wtemp;
              // do not sample here
              //LambdaHw.row(indexing_obs(u)(ix)) = w.row(indexing_obs(u)(ix)) * Lambda.t();
            }
          }
          sample_nonreference_w(u, data, rand_norm_mat);
        } 
        
      } 
    }
  }
  LambdaHw = w * Lambda.t();
  
  if(false || verbose & debug){
    end_overall = std::chrono::steady_clock::now();
    Rcpp::Rcout << "[gibbs_sample_w] gibbs loops "
                << std::chrono::duration_cast<std::chrono::microseconds>(end_overall - start_overall).count()
                << "us. " << "\n";
  }
}

void Meshed::predict(){
  start_overall = std::chrono::steady_clock::now();
  if(predict_group_exists == 1){
    message("[predict] start ");
#ifdef _OPENMP
    //***#pragma omp parallel for 
#endif
    for(int i=0; i<u_predicts.n_elem; i++){ //*** subset to blocks with NA
      int u = u_predicts(i);// u_predicts(i);
      // only predictions at this block. 
      arma::uvec predict_parent_indexing, cx;
      arma::cube Kxxi_parents;
      
      if((block_ct_obs(u) > 0) & forced_grid){
        // this is a reference set with some observed locations
        predict_parent_indexing = indexing(u); // uses knots which by construction include all k processes
        int ccfound = findcc(u);
        CviaKron_HRj_chol_bdiag_wcache(Hpred(i), Rcholpred(i), param_data.Kxxi_cache(ccfound), na_1_blocks(u),
                                       coords, indexing_obs(u), predict_parent_indexing, k, param_data.theta, matern);
      } else {
        // no observed locations, use line of sight
        predict_parent_indexing = parents_indexing(u);
        CviaKron_HRj_chol_bdiag(Hpred(i), Rcholpred(i), Kxxi_parents,
                                na_1_blocks(u),
                                coords, indexing_obs(u), predict_parent_indexing, k, param_data.theta, matern);
      }
      
      //Rcpp::Rcout << "step 1 "<< endl;
      
      for(int ix=0; ix<indexing_obs(u).n_elem; ix++){
        if(na_1_blocks(u)(ix) == 0){
          arma::mat wpars = w.rows(predict_parent_indexing);
          
          arma::rowvec wtemp = arma::sum(arma::trans(Hpred(i).slice(ix)) % wpars, 0) + 
            arma::trans(Rcholpred(i).col(ix)) % rand_norm_mat.row(indexing_obs(u)(ix));
          
          w.row(indexing_obs(u)(ix)) = wtemp;
          wU.row(indexing_obs(u)(ix)) = wtemp;
          
          LambdaHw.row(indexing_obs(u)(ix)) = w.row(indexing_obs(u)(ix)) * Lambda.t();
        }
      }
      
      //Rcpp::Rcout << "done" << endl;

      
    }
    
    if(verbose & debug){
      end_overall = std::chrono::steady_clock::now();
      Rcpp::Rcout << "[predict] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end_overall - start_overall).count()
                  << "us. ";
    }
  }
}

void Meshed::predicty(){
  int n = XB.n_rows;
  yhat.fill(0);
  
  arma::mat Lw = wU*Lambda.t();
  for(int j=0; j<q; j++){
    arma::vec linear_predictor = XB.col(j) + Lw.col(j);
    if(familyid(j) == 0){
      // gaussian
      yhat.col(j) = linear_predictor + pow(1.0/tausq_inv(j), .5) * arma::randn(n);
    }
    if(familyid(j) == 1){
      // poisson
      yhat.col(j) = vrpois(exp(linear_predictor));
    }
    if(familyid(j) == 2){
      // binomial
      yhat.col(j) = vrbern(1.0/(1.0 + exp(-linear_predictor)));
    }
  }
}

void Meshed::hmc_sample_w(MeshDataLMC& data){
  if(verbose & debug){
    Rcpp::Rcout << "[hmc_sample_w] " << endl;
  }
  
  start_overall = std::chrono::steady_clock::now();
  double part1 = 0;
  double part2 = 0;
  
  int nuts_timer = 0;
  int copy_timer = 0;
  
  arma::mat offset_for_w = offsets + XB;
  
  for(int g=0; g<n_gibbs_groups; g++){
#ifdef _OPENMP
    //***#pragma omp parallel for 
#endif
    for(int i=0; i<u_by_block_groups(g).n_elem; i++){
      int u = u_by_block_groups(g)(i);
      
      if((block_ct_obs(u) > 0)){
        
        //if(compute_block(predicting, hmc_dist_params.at(u).block_ct_obs, rfc_dep)){
        
        //Rcpp::Rcout << u << " step 1" << endl;
        //if(hmc_dist_params.at(u).block_ct_obs > 0){ // sample using nuts 
        //message("non-pure prediction block");
        //arma::mat offset_for_w = offsets.rows(indexing(u)) + XB.rows(indexing(u));
        
        //message("step 2");
        //start = std::chrono::steady_clock::now();
        
        hmc_dist_params.at(u).update_mv(offset_for_w, 1.0/tausq_inv, Lambda);
        
        
        
        if(parents(u).n_elem > 0){
          hmc_dist_params.at(u).Kxxi = (*data.w_cond_prec_parents_ptr.at(u));
          hmc_dist_params.at(u).Kcx = (*data.w_cond_mean_K_ptr.at(u));
          hmc_dist_params.at(u).w_parents = w.rows(parents_indexing(u));
          
        }
        
        if(forced_grid){
          hmc_dist_params.at(u).Hproject = data.Hproject(u);
        }
        //message("step 3");
        hmc_dist_params.at(u).Ri = (*data.w_cond_prec_ptr.at(u));
        //hmc_dist_params.at(u).p_ix = parents_indexing(u);
        
        hmc_dist_params.at(u).nu = data.nu;
        //end = std::chrono::steady_clock::now();
        //copy_timer += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        //Rcpp::Rcout << "stored pointer values " << endl;
        
        hmc_dist_params.at(u).parKxxpar = arma::zeros(k);
          
        if(parents_indexing(u).n_rows > 0){
          hmc_dist_params.at(u).Kcxpar = arma::zeros(hmc_dist_params.at(u).Kcx.n_rows, k);
          for(int j=0; j<k; j++){
            if(latent == "student"){
              hmc_dist_params.at(u).parKxxpar(j) = arma::conv_to<double>::from(
                hmc_dist_params.at(u).w_parents.col(j).t() * 
                  hmc_dist_params.at(u).Kxxi.slice(j) * 
                  hmc_dist_params.at(u).w_parents.col(j));
            }
            hmc_dist_params.at(u).Kcxpar.col(j) = hmc_dist_params.at(u).Kcx.slice(j) * hmc_dist_params.at(u).w_parents.col(j);
          }
          
        } else {
          hmc_dist_params.at(u).parKxxpar.fill(-1);
        }
          
        hmc_dist_params.at(u).woKoowo = arma::zeros(k, hmc_dist_params.at(u).num_children);
        
        for(int c=0; c<hmc_dist_params.at(u).num_children; c++ ){
          int child = children(u)(c);
          //Rcpp::Rcout << "child [" << child << "]\n";
          arma::uvec c_ix = indexing(child);
          arma::uvec pofc_ix = parents_indexing(child);
          
          arma::uvec pofc_ix_x = u_is_which_col_f(u)(c)(0);
          arma::uvec pofc_ix_other = u_is_which_col_f(u)(c)(1);
          
          arma::mat w_childs_parents = w.rows(pofc_ix);
          arma::mat w_otherparents = w_childs_parents.rows(pofc_ix_other);
          
          
          //Rcpp::Rcout << "step 1 " << endl;
          // store
          //start = std::chrono::steady_clock::now();
          hmc_dist_params.at(u).dim_of_pars_of_children(c) = parents_indexing(child).n_rows;
          hmc_dist_params.at(u).w_child(c) = w.rows(c_ix);
          hmc_dist_params.at(u).Ri_of_child(c) = (*data.w_cond_prec_ptr.at(child));
          hmc_dist_params.at(u).Kcx_x(c) = cube_cols_ptr(data.w_cond_mean_K_ptr.at(child), pofc_ix_x);
          arma::cube Kcx_other = //(*param_data.w_cond_mean_K.at(child)).cols(pofc_ix_other);
            cube_cols_ptr(data.w_cond_mean_K_ptr.at(child), pofc_ix_other);
          //end = std::chrono::steady_clock::now();
          //copy_timer += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
          
          hmc_dist_params.at(u).Kco_wo(c) = cube_times_mat(Kcx_other, w_otherparents);
          
          //Rcpp::Rcout << "step 2 " << endl;
          arma::cube Kxxi_other;
          if(latent == "student"){
            Kxxi_other = subcube_ptr(data.w_cond_prec_parents_ptr.at(child),
                                     pofc_ix_other, pofc_ix_other);
            arma::cube Kxxi_xo = subcube_ptr(data.w_cond_prec_parents_ptr.at(child),
                                             pofc_ix_x, pofc_ix_other);
            hmc_dist_params.at(u).Kxxi_x(c) = subcube_ptr(data.w_cond_prec_parents_ptr.at(child),
                               pofc_ix_x, pofc_ix_x);
            hmc_dist_params.at(u).Kxo_wo(c) = cube_times_mat(Kxxi_xo, w_otherparents);
          }
          
          
          //Rcpp::Rcout << "step 3 " << endl;
          if(w_otherparents.n_rows > 0){
            if(latent == "student"){
              for(int j=0; j<k; j++){
                hmc_dist_params.at(u).woKoowo(j, c) = arma::conv_to<double>::from(
                  w_otherparents.col(j).t() * Kxxi_other.slice(j) * w_otherparents.col(j));
              }
              
            }
            if(latent == "gaussian"){
              hmc_dist_params.at(u).woKoowo.col(c).fill(0);
            }
          } else {
            hmc_dist_params.at(u).woKoowo.col(c).fill(-1);
          }
        }
        
        // -----------------------------------------------
          
        //end = std::chrono::steady_clock::now();
        //part1 += std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        //message("3 block");
        // HMC/NUTS steps
        //message("step 5");
        
        arma::mat w_current = w.rows(indexing(u));
        
        // adapting eps
        hmc_eps_adapt.at(u).step();
        if((hmc_eps_started_adapting(u) == 0) & (hmc_eps_adapt.at(u).i==10)){
          // wait a few iterations before starting adaptation
          //message("find reasonable");
          hmc_eps(u) = find_reasonable_stepsize(w_current, hmc_dist_params.at(u));
          //message("found reasonable");
          AdaptE new_adapting_scheme(hmc_eps(u), 1e6);
          hmc_eps_adapt.at(u) = new_adapting_scheme;
          hmc_eps_started_adapting(u) = 1;
        }
        
        
        arma::mat w_temp;
        //if(w_sampling_method == "nuts"){
          //Rcpp::Rcout << "sampling nuts" << endl;
          //w_temp = sample_one_nuts_cpp(w_current, hmc_dist_params.at(u), hmc_eps_adapt.at(u)); 
        //} else {
          //Rcpp::Rcout << "mala " << endl;
          //Rcpp::Rcout << arma::size(w_current) << endl;
          w_temp = sample_one_mala_cpp(w_current, hmc_dist_params.at(u), hmc_eps_adapt.at(u), true, true); 
          //Rcpp::Rcout << " done mala " << endl;
        //}
        
        hmc_eps(u) = hmc_eps_adapt.at(u).eps;
        
        w.rows(indexing(u)) = w_temp;//arma::trans(arma::mat(w_temp.memptr(), q, w_temp.n_elem/q));
        wU.rows(indexing(u)) = w.rows(indexing(u));
        
        if(forced_grid){
          for(int ix=0; ix<indexing_obs(u).n_elem; ix++){
            if(na_1_blocks(u)(ix) == 1){
              arma::mat wtemp = arma::sum(arma::trans(data.Hproject(u).slice(ix) % arma::trans(w.rows(indexing(u)))), 0);
              
              w.row(indexing_obs(u)(ix)) = wtemp;
              wU.row(indexing_obs(u)(ix)) = wtemp;
              //LambdaHw.row(indexing_obs(u)(ix)) = w.row(indexing_obs(u)(ix)) * Lambda.t();
            }
          }
        }
        
      
      }
    }
  }
  LambdaHw = w * Lambda.t();
  
  if(verbose & debug){
    end_overall = std::chrono::steady_clock::now();
    Rcpp::Rcout << "[hmc_sample_w] "
                << std::chrono::duration_cast<std::chrono::microseconds>(end_overall - start_overall).count()
                << "us. " << "\n";
    Rcpp::Rcout << "-- of which " << nuts_timer << " from nuts " << endl;
    Rcpp::Rcout << "-- of which " << copy_timer << " from copying " << endl;
  }
  
}