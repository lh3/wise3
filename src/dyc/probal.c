#ifdef _cplusplus
extern "C" {
#endif
#include "probal.h"





/* Function:  write_probabilistic_models(dfp,gm,dpi)
 *
 * Descrip:    Makes all the probabilistic routines
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 20 "probal.dy"
void write_probabilistic_models(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  write_score_only_sum(dfp,gm,dpi);

  write_expl_mat_sum(dfp,gm,dpi);

  write_backward_mat_sum(dfp,gm,dpi);
}

/* Function:  write_forwardbackward_struct(dfp,dpi,gm)
 *
 * Descrip:    Makes the forwardbackward struct
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 32 "probal.dy"
void write_forwardbackward_struct(DYNFILE * dfp,DPImplementation * dpi,GenericMatrix * gm)
{
  StructHolder * sh;
  StructElement * temp;
  char buffer[MAXLINE];

  sh = StructHolder_alloc_std();

  assert(sh != NULL);

  sprintf(buffer,"%s_Posterior",gm->name);
  sh->name = stringalloc(buffer);

  sprintf(buffer,"%s *",gm->name);
  temp = basic_add_StructElement(sh,"forward",buffer);
  temp->def = stringalloc("NULL");
  temp = basic_add_StructElement(sh,"backward",buffer);
  temp->def = stringalloc("NULL");

  
  write_StructHolder_header(dfp,sh);

  write_StructHolder_function(dfp,sh,NULL);


  free_StructHolder(sh);


}

/* Function:  write_backward_mat_sum(dfp,gm,dpi)
 *
 * Descrip:    Makes the backward matrix over all paths
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 65 "probal.dy"
void write_backward_mat_sum(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  int i_state;
  int j_source;
  int k;
  FuncInfo * fi;
  char * arg_str;
  char * chainstr;

  char * mat_string;


  CellState * start = NULL;
    

  for(k=0;k<gm->spec_len;k++) {
    if( gm->special[k]->is_start == TRUE ) {
      if( start != NULL ) {
	assert("Cannot have two start states for probabilistic backward routines" != 0 );
      } else {
	start = gm->special[k];
      }
    }
  }


  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"backward_logsum_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the matrix over all paths");
  add_line_to_Ftext(fi->ft,"This is using a logsum method to sort it all out");

  arg_str = get_argstr_GenericMatrix(gm);
  chainstr = get_chainstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  start_function_FuncInfo(fi,dfp,"%s * backward_logsum_%s(%s,DPRunImpl * dpri)",gm->name,gm->name,arg_str);
  ckfree(arg_str);

  expr(dfp,"%s * mat;",gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"int tot;");
  expr(dfp,"int num;");

  expr(dfp,"int max_score;");
  expr(dfp,"int min_score;");

  add_break(dfp);



  expr(dfp,"assert((mat=allocate_%s_only(%s)) != NULL)",gm->name,chainstr);
  expr(dfp,"mat->basematrix = BaseMatrix_alloc_matrix_specials_score_offset((mat->lenj+%d)*%d,(mat->leni+%d),%d,mat->lenj+%d)",gm->window_j,gm->len,gm->window_i,gm->spec_len,gm->window_j);
  expr(dfp,"assert(mat->basematrix != NULL)");

  expr(dfp,"leni = mat->leni;");
  expr(dfp,"lenj = mat->lenj;");
  expr(dfp,"mat->basematrix->type = BASEMATRIX_TYPE_EXPLICIT;");
  expr(dfp,"init_%s(mat)",gm->name);
  
  
  /*** make num and tot ***/
  
  expr(dfp,"tot = leni * lenj;");
  expr(dfp,"num = 0;");
  
  /*** swap over specials ***/

  add_block_comment(dfp,"On the backward view, we need to have 0 at end, and NEGI at start");
  expr(dfp,"for(j= -%d;j<lenj;j++)",gm->window_j);
  startbrace(dfp);
  for(i_state = 0;i_state < gm->spec_len;i_state++) {
    auto CellState * state;

    state = gm->special[i_state];

    if( state->is_start == TRUE ) {
      /* invert */
      expr(dfp,"%s_EXPL_SPECIAL(mat,0,j,%s) = NEGI",gm->name,state->name);
    }
    if( state->is_end == TRUE ) {
      /* invert */
      expr(dfp,"%s_EXPL_SPECIAL(mat,0,j,%s) = 0",gm->name,state->name);
    }
  }
  closebrace(dfp);

  /*** see if there any specials to specials to do ***/
  
  add_break(dfp);
  expr(dfp,"start_reporting(\"%s Matrix calculation: \");",gm->name);
  
  add_break(dfp);

  expr(dfp,"for(j=lenj-1;j>=0;j--)");
  startbrace(dfp);
  expr(dfp,"auto int score");
  expr(dfp,"auto int temp");



  /* now handle the possibility of an underflow or overflow */


  add_block_comment(dfp,"We look for underflow or overflow on this j column.");
  add_block_comment(dfp,"We do this at the start of the loop because in the backwards case at this point we know nothing will _update_ this column");

  expr(dfp,"min_score = SCORE_OVERFLOW;");
  expr(dfp,"max_score = SCORE_UNDERFLOW;");
  expr(dfp,"for(i=0;i<mat->leni;i++)");
  startbrace(dfp);
  for(i_state = 0;i_state<gm->len;i_state++) {
    expr(dfp,"if( %s_EXPL_MATRIX(mat,i,j,%s) > max_score)",gm->name,gm->state[i_state]->name);
    hang_expr(dfp,"max_score =  %s_EXPL_MATRIX(mat,i,j,%s)",gm->name,gm->state[i_state]->name);
  
    expr(dfp,"if( %s_EXPL_MATRIX(mat,i,j,%s) < min_score)",gm->name,gm->state[i_state]->name);
    hang_expr(dfp,"min_score =  %s_EXPL_MATRIX(mat,i,j,%s)",gm->name,gm->state[i_state]->name);
  }

  closebrace(dfp);

  for(i_state = 0;i_state<gm->spec_len;i_state++) {
    expr(dfp,"if( %s_EXPL_SPECIAL(mat,i,j,%s) > max_score)",gm->name,gm->special[i_state]->name);
    hang_expr(dfp,"max_score =  %s_EXPL_SPECIAL(mat,i,j,%s)",gm->name,gm->special[i_state]->name);
    
    expr(dfp,"if( %s_EXPL_SPECIAL(mat,i,j,%s) < min_score)",gm->name,gm->special[i_state]->name);
    hang_expr(dfp,"min_score =  %s_EXPL_SPECIAL(mat,i,j,%s)",gm->name,gm->special[i_state]->name);
  }

  expr(dfp,"if( max_score > SCORE_OVERFLOW )");
  startbrace(dfp);
  expr(dfp,"if( min_score < SCORE_UNDERFLOW)");
  hang_expr(dfp,"fatal(\"Both overflow and underflow on the same column\")");
  expr(dfp,"mat->basematrix->score_offset[j] = -1");
  expr(dfp,"for(i=0;i<mat->leni;i++)");
  startbrace(dfp);
  for(i_state = 0;i_state<gm->len;i_state++) {
    expr(dfp,"%s_EXPL_MATRIX(mat,i,j,%s) -= SCORE_OVERFLOW",gm->name,gm->state[i_state]->name);
  }
  closebrace(dfp);

  for(i_state = 0;i_state<gm->spec_len;i_state++) {
    expr(dfp,"%s_EXPL_SPECIAL(mat,i,j,%s) -= SCORE_OVERFLOW",gm->name,gm->special[i_state]->name);
  }
  closebrace(dfp);

  
  expr(dfp,"if( min_score < SCORE_UNDERFLOW )");
  startbrace(dfp);
  expr(dfp,"mat->basematrix->score_offset[j] = +1");
  expr(dfp,"for(i=0;i<mat->leni;i++)");
  startbrace(dfp);
  for(i_state = 0;i_state<gm->len;i_state++) {
    expr(dfp,"%s_EXPL_MATRIX(mat,i,j,%s) -= SCORE_UNDERFLOW",gm->name,gm->state[i_state]->name);
  }
  closebrace(dfp);

  for(i_state = 0;i_state<gm->spec_len;i_state++) {
    expr(dfp,"%s_EXPL_SPECIAL(mat,i,j,%s) -= SCORE_UNDERFLOW",gm->name,gm->special[i_state]->name);
  }
  closebrace(dfp);

  

  expr(dfp,"for(i=leni-1;i>=0;i--)");
  startbrace(dfp);
  
  
  expr(dfp,"if( num%%1000 == 0 )");
  hang_expr(dfp,"log_full_error(REPORT,0,\"[%%7d] Cells %%2d%%%%%%%%\",num,num*100/tot);");
  

  expr(dfp,"num++;"); 

  /* because it is all backwards, cannot use the standard codes */

  /* the special states first, then the main states */


  



  for(i_state = 0;i_state < gm->spec_len;i_state++) {
    auto CellState * state;
    state = gm->special[i_state];
    add_break(dfp);

    if( state->specialtospecial == TRUE ) {
      add_block_comment(dfp,"Have not handled special to special yet!");
      expr(dfp,"assert(\"Bad Dynamite code cannot handle special backwards\")");
    }

  }
   
  /* for each species state, if it has any main state sources,
       we need to handle the reverse calculation now */
  for(i_state = 0;i_state < gm->len;i_state++) {
    auto CellState * state;
    state = gm->state[i_state];
    
    for(k = 0;k<gm->spec_len;k++) {
      auto CellState * special;
      special = gm->special[k];
      for(j_source = 0;j_source < special->len;j_source++) {
	if( strcmp(special->source[j_source]->state_source,state->name) == 0 ) {
	  /* forwards from state to special - backwards need to reverse */
	  add_block_comment(dfp,"special state %s recieves from %s",gm->special[k]->name,state->name);
	
	  if( special->source[j_source]->position != SOURCE_POS_ALL ) {
	    add_block_comment(dfp,"Has restricted position");
	    expr(dfp,"if( %s ) ",source_allowed_statement(special->source[j_source]->position,special->source[j_source]->offi,special->source[j_source]->offj));
	    startbrace(dfp);
	  }

	  expr(dfp,"temp = %s_EXPL_SPECIAL(mat,i,j,%s) + %s + (%s)",gm->name,special->name,special->source[j_source]->calc_expr,
	       special->calc_expr == NULL ? "0" : special->calc_expr);
	  expr(dfp,"%s_EXPL_MATRIX(mat,i-%d,j-%d,%s) = %s(%s_EXPL_MATRIX(mat,i-%d,j-%d,%s),temp)",
	       gm->name,special->source[j_source]->offi,
	       special->source[j_source]->offj,special->source[j_source]->state_source,
	       dpi->calcfunc,
	       gm->name,special->source[j_source]->offi,special->source[j_source]->offj,special->source[j_source]->state_source);

	  if( special->source[j_source]->position != SOURCE_POS_ALL ) {
	    closebrace(dfp);
	  }
	  
	}
      }
    }
  }

  /* now do the main states. They are "written" in the forward
     direction, so have to take the end index, do the calculation
     and add onto the final set */

  for(i_state = 0;i_state < gm->len ;i_state++) {
    auto CellState * state;
    state = gm->state[i_state];
    add_break(dfp);
    add_block_comment(dfp,"Doing calculations which end at this state");
    
    for(j_source = 0;j_source < state->len;j_source++) {
      auto CellSource * source;

      source = state->source[j_source];

      if( source->position != SOURCE_POS_ALL ) {
	add_block_comment(dfp,"Has restricted position");
	if( strcmp(source->state_source,start->name) == 0 ) {
	  expr(dfp,"if( %s )",source_allowed_statement(source->position,source->offi,source->offj-1));
	} else {
	  expr(dfp,"if( %s )",source_allowed_statement(source->position,source->offi,source->offj));
	}
	startbrace(dfp);
      }
      add_block_comment(dfp,"Reversed from state %s to state %s",state->name,source->state_source);

      if( source->isspecial == TRUE ) {
	mat_string = "SPECIAL";
      } else {
	mat_string = "MATRIX";
      }

      expr(dfp,"temp = %s_EXPL_MATRIX(mat,i,j,%s) + %s + (%s)",gm->name,state->name,source->calc_expr,
	   state->calc_expr == NULL ? "0" : state->calc_expr);
      expr(dfp,"%s_EXPL_%s(mat,i-%d,j-%d,%s) = %s(%s_EXPL_%s(mat,i-%d,j-%d,%s),temp)",
	   gm->name,mat_string,source->offi,source->offj,source->state_source,
	   dpi->calcfunc,
	   gm->name,mat_string,source->offi,source->offj,source->state_source
	   );
      
      if( state->source[j_source]->position != SOURCE_POS_ALL) {
	closebrace(dfp);
      }
    }
  }

  closebrace(dfp); /* over i */




  /* finish up */


  closebrace(dfp); /* over j */
  
  
  
  expr(dfp,"stop_reporting()");
    
  expr(dfp,"return mat");
  
  close_function(dfp);
  
  add_break(dfp);

}


/* Function:  write_expl_mat_sum(dfp,gm,dpi)
 *
 * Descrip:    Makes the explicit matrix over all paths
 *             construction method
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 371 "probal.dy"
void write_expl_mat_sum(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  FuncInfo * fi;
  char * arg_str;
  char * chainstr;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"forward_logsum_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the matrix over all paths");
  add_line_to_Ftext(fi->ft,"This is using a logsum method to sort it all out");

  arg_str = get_argstr_GenericMatrix(gm);
  chainstr = get_chainstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  start_function_FuncInfo(fi,dfp,"%s * forward_logsum_%s(%s,DPRunImpl * dpri)",gm->name,gm->name,arg_str);
  ckfree(arg_str);

  expr(dfp,"%s * mat;",gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"int tot;");
  expr(dfp,"int num;");
  add_break(dfp);

  
  expr(dfp,"assert((mat=allocate_%s_only(%s)) != NULL)",gm->name,chainstr);
  expr(dfp,"mat->basematrix = BaseMatrix_alloc_matrix_specials_score_offset((mat->lenj+%d)*%d,(mat->leni+%d),%d,mat->lenj+%d)",gm->window_j,gm->len,gm->window_i,gm->spec_len,gm->window_j);
  expr(dfp,"assert(mat->basematrix != NULL)");

  expr(dfp,"leni = mat->leni;");
  expr(dfp,"lenj = mat->lenj;");
  expr(dfp,"mat->basematrix->type = BASEMATRIX_TYPE_EXPLICIT;");

  expr(dfp,"init_%s(mat)",gm->name);
  
  /*** make num and tot ***/
  
  expr(dfp,"tot = leni * lenj;");
  expr(dfp,"num = 0;");
  

  /*** see if there any specials to specials to do ***/
  
  add_break(dfp);
  expr(dfp,"start_reporting(\"%s Matrix calculation: \");",gm->name);
  
  expr(dfp,"for(j=0;j<lenj;j++)");
  startbrace(dfp);
  expr(dfp,"auto int score");
  expr(dfp,"auto int temp");
  expr(dfp,"for(i=0;i<leni;i++)");
  startbrace(dfp);

  expr(dfp,"if( num%%1000 == 0 )");
  hang_expr(dfp,"log_full_error(REPORT,0,\"[%%7d] Cells %%2d%%%%%%%%\",num,num*100/tot);");
  

  expr(dfp,"num++;"); 

  gm->calcfunc = dpi->calcfunc;

  write_score_block(dfp,gm,"EXPL_MATRIX","mat","EXPL_SPECIAL",TRUE);


  closebrace(dfp);

  write_special_block(dfp,gm,"EXPL_MATRIX","EXPL_SPECIAL",NULL);

  gm->calcfunc = NULL;
  closebrace(dfp);
  
  /*** stop reporting ***/
  
  expr(dfp,"stop_reporting()");
    
  expr(dfp,"return mat");
  
  close_function(dfp);
  
  add_break(dfp);
}  

/* Function:  write_score_only_sum(dfp,gm,dpi)
 *
 * Descrip:    Makes the single one-on-one over all paths
 *             searching method
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 * Arg:        dpi [UNKN ] Undocumented argument [DPImplementation *]
 *
 */
# line 459 "probal.dy"
void write_score_only_sum(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  int i;
  FuncInfo * fi;
  char * arg_str;
  char * chain_str;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"score_only_logsum_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the score over all paths");
  add_line_to_Ftext(fi->ft,"This is using a logsum method to sort it all out");

  arg_str = get_argstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  start_function_FuncInfo(fi,dfp,"Score score_only_logsum_%s(%s)",gm->name,arg_str);
  ckfree(arg_str);

  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int bestscore = 0;");
  expr(dfp,"int k;");
  expr(dfp,"%s * mat",gm->name);


  if(gm->qtype != NULL && gm->qtype->maxlen != NULL) {
    expr(dfp,"int internal_matrix[%d][(%s+%d) * %d];",gm->window_j+1,gm->qtype->maxlen,gm->window_i,gm->len);
    expr(dfp,"int internal_specials[%d][%d];",gm->window_j+1,gm->spec_len);
  }

  
  add_break(dfp);
  chain_str = get_chainstr_GenericMatrix(gm);
  expr(dfp,"mat = allocate_%s_only(%s);",gm->name,chain_str);
  ckfree(chain_str);

  expr(dfp,"if( mat == NULL )");
  startbrace(dfp);
  warn_expr(dfp,"Memory allocation error in the db search - unable to communicate to calling function. this spells DISASTER!");
  expr(dfp,"return NEGI");
  closebrace(dfp);

  if(gm->qtype != NULL && gm->qtype->maxlen != NULL) {
    add_block_comment(dfp,"Ok,don't need to allocate matrix as it is internal, because we have a max length");
  } else {
    expr(dfp,"if((mat->basematrix = BaseMatrix_alloc_matrix_and_specials(%d,(mat->leni + %d) * %d,%d,%d)) == NULL)",gm->window_j+1,gm->window_i,gm->len,gm->window_j+1,gm->spec_len);
    startbrace(dfp);
    expr(dfp,"warn(\"Score only matrix for %s cannot be allocated, (asking for %d  by %%d  cells)\",mat->leni*%d);",gm->name,gm->window_j,gm->len);
    expr(dfp,"mat = free_%s(mat)",gm->name);
    expr(dfp,"return 0;");
    closebrace(dfp);
    expr(dfp,"mat->basematrix->type = BASEMATRIX_TYPE_VERYSMALL;");
  }

  add_break(dfp);

  add_block_comment(dfp,"Now, initiate matrix");

  expr(dfp,"for(j=0;j<%d;j++)",gm->window_j+2);
  startbrace(dfp);
  expr(dfp,"for(i=(-%d);i<mat->leni;i++)",gm->window_i);
  startbrace(dfp);
  expr(dfp,"for(k=0;k<%d;k++)",gm->len);
  hang_expr(dfp,"%s_VSMALL_MATRIX(mat,i,j,k) = NEGI;",gm->name);
  closebrace(dfp);
  for(i=0;i<gm->spec_len;i++) {
    expr(dfp,"%s_VSMALL_SPECIAL(mat,i,j,%s) = %s;",gm->name,gm->special[i]->name,gm->special[i]->def_score);
  }
  closebrace(dfp);

  add_break(dfp);

  add_block_comment(dfp,"Ok, lets do-o-o-o-o it");

  add_break(dfp);

  expr(dfp,"for(j=0;j<mat->lenj;j++)");
  startbrace_tag(dfp,"for all target positions");
  expr(dfp,"auto int score");
  expr(dfp,"auto int temp");
  expr(dfp,"for(i=0;i<mat->leni;i++)");
  startbrace_tag(dfp,"for all query positions");
  
  gm->calcfunc = dpi->calcfunc;
  write_score_block(dfp,gm,"VSMALL_MATRIX","mat","VSMALL_SPECIAL",TRUE);
  gm->calcfunc = NULL;

  closebrace(dfp);

  add_break(dfp);

  gm->calcfunc = dpi->calcfunc;
  write_special_block(dfp,gm,"VSMALL_MATRIX","VSMALL_SPECIAL",NULL);
  gm->calcfunc = NULL;

  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"mat = free_%s(mat)",gm->name);

  expr(dfp,"return bestscore;");

  close_function(dfp);

  add_break(dfp);
}






# line 593 "probal.c"

#ifdef _cplusplus
}
#endif
