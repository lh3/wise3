#ifdef _cplusplus
extern "C" {
#endif
#include "dynafunc.h"



# line 25 "dynafunc.dy"
void one_shot_aln_func(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  FuncInfo * fi;
  char * arg_str;
  char * chainstr;


  /*** prepare function information ***/

  
  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"PackAln_bestmemory_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function chooses the best memory set-up for the alignment",gm->name);
  add_line_to_Ftext(fi->ft,"using calls to basematrix, and then implements either a large");
  add_line_to_Ftext(fi->ft,"or small memory model.");
  add_break_to_Ftext(fi->ft);
  add_line_to_Ftext(fi->ft,"It is the best function to use if you just want an alignment",gm->name);
  add_break_to_Ftext(fi->ft);
  add_line_to_Ftext(fi->ft,"If you want a label alignment, you will need");
  add_line_to_Ftext(fi->ft,"/convert_PackAln_to_AlnBlock_%s",gm->name);

  arg_str = get_argstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  
  start_function_FuncInfo(fi,dfp,"PackAln * PackAln_bestmemory_%s(%s,DPEnvelope * dpenv,DPRunImpl * dpri)",gm->name,arg_str);

  /*** clean up ***/
  ckfree(arg_str);
  chainstr= get_chainstr_GenericMatrix(gm);
  /** started function, now see what basematrix says about memory **/


  expr(dfp,"long long total;");
  expr(dfp,"%s * mat;",gm->name);
  expr(dfp,"PackAln * out;");
  expr(dfp,"DebugMatrix * de");
  expr(dfp,"DPRunImplMemory strategy;");

  expr(dfp,"assert(dpri);");
  add_break(dfp);

  expr(dfp,"total = %s->%s * %s->%s",gm->query->name,gm->query_len,gm->target->name,gm->target_len);

  expr(dfp,"if( dpri->memory == DPIM_Default ) ");
  startbrace(dfp);
  expr(dfp,"if( (total * %d * sizeof(int)) > 1000*dpri->kbyte_size)",gm->len);
  startbrace(dfp);
  expr(dfp,"strategy = DPIM_Linear;");
  closebrace(dfp);
  expr(dfp,"else");
  startbrace(dfp);
  expr(dfp,"strategy = DPIM_Explicit;");
  closebrace(dfp);
  closebrace(dfp);
  expr(dfp,"else");
  startbrace(dfp);
  expr(dfp,"strategy = dpri->memory;");
  closebrace(dfp);
  add_break(dfp);

  expr(dfp,"if( dpenv != NULL )");
  startbrace(dfp);
   expr(dfp,"if( strategy == DPIM_Explicit)");
   startbrace(dfp);
    expr(dfp,"if( (mat=allocate_Expl_%s(%s,dpri)) == NULL )",gm->name,chainstr);
    startbrace(dfp);
    warn_expr(dfp,"Unable to allocate large %s version",gm->name);
    expr(dfp,"return NULL;");
    closebrace(dfp);
    expr(dfp,"calculate_dpenv_%s(mat,dpenv)",gm->name);   
    expr(dfp,"out =  PackAln_read_Expl_%s(mat);",gm->name);
   closebrace(dfp);
   expr(dfp,"else");
   startbrace(dfp);
   expr(dfp,"mat = allocate_%s_only(%s)",gm->name,chainstr);
   expr(dfp,"calculate_shatter_%s(mat,dpenv)",gm->name);
   expr(dfp,"out = PackAln_read_Shatter_%s(mat)",gm->name);
   closebrace(dfp);
  closebrace(dfp);
  expr(dfp,"else");
  startbrace(dfp);
  expr(dfp,"if( strategy == DPIM_Linear )",gm->len);
  startbrace(dfp);
  add_block_comment(dfp,"use small implementation");
  expr(dfp,"if( (mat=allocate_Small_%s(%s)) == NULL )",gm->name,chainstr);
  startbrace(dfp);
  warn_expr(dfp,"Unable to allocate small %s version",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);
  expr(dfp,"out = PackAln_calculate_Small_%s(mat,dpenv)",gm->name);
  closebrace(dfp);
  expr(dfp,"else");
  startbrace(dfp);
  add_block_comment(dfp,"use Large implementation");
  expr(dfp,"if( (mat=allocate_Expl_%s(%s,dpri)) == NULL )",gm->name,chainstr);
  startbrace(dfp);
  warn_expr(dfp,"Unable to allocate large %s version",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);
  expr(dfp,"if( dpri->debug == TRUE)");
  startbrace(dfp);
  if( dpi->dydebug == TRUE ) {
    expr(dfp,"de = create_debug_%s();",gm->name);
    expr(dfp,"de->matrix = (void*) mat;");
    expr(dfp,"explicit_debug_%s(de,0);",gm->name);
    expr(dfp,"out =  PackAln_read_Expl_%s(mat);",gm->name);

    expr(dfp,"if( dpri->paldebug == TRUE ) ");
    hang_expr(dfp,"show_PackAln_Debug(de,out,stdout);");
    expr(dfp,"free_DebugMatrix(de);");
  } else {
    expr(dfp,"fatal(\"Asked for dydebug, but dynamite file not compiled with -g. Need to recompile dynamite source\");");
  }
  closebrace(dfp);
  expr(dfp,"else");
  startbrace(dfp);
  expr(dfp,"calculate_%s(mat)",gm->name);   
  expr(dfp,"out =  PackAln_read_Expl_%s(mat);",gm->name);
  closebrace(dfp);

  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"mat = free_%s(mat)",gm->name);

  expr(dfp,"return out;");

    
  close_function(dfp);
    
  add_break(dfp);

  ckfree(chainstr);
}

/* Function:  recalculate_PackAln_func(dfp,gm)
 *
 * Descrip:    makes the recalculate packaln function
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [const GenericMatrix *]
 *
 */
# line 165 "dynafunc.dy"
void recalculate_PackAln_func(DYNFILE * dfp,const GenericMatrix * gm)
{
  FuncInfo * fi;
  /* ArgInfo  * ai; */
  int i;
  int j;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"recalculate_PackAln_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function recalculates the PackAln structure produced by %s",gm->name);
  add_line_to_Ftext(fi->ft,"For example, in linear space methods this is used to score them",gm->name);

  start_function_FuncInfo(fi,dfp,"void recalculate_PackAln_%s(PackAln * pal,%s * mat)",gm->name,gm->name);

  expr(dfp,"int i,j,k,offi,offj;");
  expr(dfp,"PackAlnUnit * prev;");
  expr(dfp,"PackAlnUnit * pau;");
  add_break(dfp);


  expr(dfp,"for(k=1,prev=pal->pau[0];k < pal->len;k++,prev=pau)");
  startbrace(dfp);
  expr(dfp,"pau = pal->pau[k];");
  expr(dfp,"i = pau->i;");
  expr(dfp,"j = pau->j;");
  expr(dfp,"offi = pau->i - prev->i");
  expr(dfp,"offj = pau->j - prev->j");
  expr(dfp,"switch(pau->state)");
  startbrace(dfp);
  for(i=0;i<gm->len;i++) {
    expr(dfp,"case %s : ",gm->state[i]->name);
    startcase(dfp);


    /*** about to move to if statements... ***/


    for(j=0;j<gm->state[i]->len;j++) {
      if( gm->state[i]->source[j]->isspecial == TRUE) 
	expr(dfp,"if( offj == %d && prev->state == (%s+%d) )",gm->state[i]->source[j]->offj,gm->state[i]->source[j]->state_source,gm->len);
      else 
	expr(dfp,"if( offi == %d && offj == %d && prev->state == %s )",gm->state[i]->source[j]->offi,gm->state[i]->source[j]->offj,gm->state[i]->source[j]->state_source);
      startbrace(dfp);
      expr(dfp,"pau->score = %s + (%s)",gm->state[i]->source[j]->calc_expr,gm->state[i]->calc_expr == NULL ? "0" : gm->state[i]->calc_expr);
      expr(dfp,"continue;");
      closebrace(dfp);
    }
    expr(dfp,"warn(\"In recaluclating PackAln with state %s, from [%%d,%%d,%%d], got a bad source state. Error!\",offi,offj,prev->state);",gm->state[i]->name);
    expr(dfp,"break;");
    closecase(dfp);
  }

  for(i=0;i<gm->spec_len;i++) {
    expr(dfp,"case (%s+%d) : ",gm->special[i]->name,gm->len);
    startcase(dfp);
    for(j=0;j<gm->special[i]->len;j++) {
      if( gm->special[i]->source[j]->isspecial == TRUE) 
	expr(dfp,"if( offj == %d && prev->state == (%s+%d) )",gm->special[i]->source[j]->offj,gm->special[i]->source[j]->state_source,gm->len);
      else 
	expr(dfp,"if( offj == %d && prev->state == %s )",gm->special[i]->source[j]->offj,gm->special[i]->source[j]->state_source);
      startbrace(dfp);

      if( gm->special[i]->source[j]->isspecial == FALSE) { 
	add_block_comment(dfp,"i here comes from the previous state ;) - not the real one");
	expr(dfp,"i = prev->i;");
      }
      expr(dfp,"pau->score = %s + (%s)",gm->special[i]->source[j]->calc_expr,gm->special[i]->calc_expr == NULL ? "0" : gm->special[i]->calc_expr);
      expr(dfp,"continue;");
      closebrace(dfp);
    }
    warn_expr(dfp,"In recaluclating PackAln with state %s, got a bad source state. Error!",gm->special[i]->name);
    expr(dfp,"break;");
    closecase(dfp);
  }

  expr(dfp,"default : ");
  startcase(dfp);
  warn_expr(dfp,"In recaluclating PackAln got a bad recipient state. Error!");
  closecase(dfp);
  closebrace(dfp); /* end of switch on pau->state */
  expr(dfp,"prev = pau;");
  closebrace(dfp);

  expr(dfp,"return;");
  close_function(dfp);
}
  


    

  
 
/* Function:  alloc_expl_func_GenericMatrix(dfp,gm)
 *
 * Descrip:    makes the allocate_Expl_xxx function.
 *             This calls allocate_xxx_only function
 *             (made by /write_safe_alloc_function)
 *             and then allocates basematrix stuff as well.
 *
 *
 * Arg:        dfp [UNKN ] dynamite file pointer [DYNFILE *]
 * Arg:         gm [READ ] generic matrix structure [const GenericMatrix *]
 *
 */
# line 266 "dynafunc.dy"
void alloc_expl_func_GenericMatrix(DYNFILE * dfp,const GenericMatrix * gm)
{
  FuncInfo * fi;
  char * arg_str;
  char * chain_str;


  /*** prepare function information ***/

  
  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"allocate_Expl_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function allocates the %s structure",gm->name);
  add_line_to_Ftext(fi->ft,"and the basematrix area for explicit memory implementations");
  add_line_to_Ftext(fi->ft,"It calls /allocate_%s_only",gm->name);

  arg_str = get_argstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);

  


  start_function_FuncInfo(fi,dfp,"%s * allocate_Expl_%s(%s,DPRunImpl * dpri)",gm->name,gm->name,arg_str);


  /*** clean up ***/
  ckfree(arg_str);



  /*** into function body ***/


  expr(dfp,"%s * out;",gm->name);
  
  add_break(dfp);
  chain_str = get_chainstr_GenericMatrix(gm);
  expr(dfp,"out = allocate_%s_only(%s);",gm->name,chain_str);
  ckfree(chain_str);

  expr(dfp,"if( out == NULL )");
  hang_expr(dfp,"return NULL;");

  expr(dfp,"if( dpri->should_cache == TRUE )");
  startbrace(dfp);
  expr(dfp,"if( dpri->cache != NULL )");
  startbrace(dfp);
  expr(dfp,"if( dpri->cache->maxleni >= (out->lenj+%d)*%d && dpri->cache->maxlenj >= (out->leni+%d))",gm->window_j,gm->len,gm->window_i);
  hang_expr(dfp,"out->basematrix = hard_link_BaseMatrix(dpri->cache)");
  expr(dfp,"else");
  hang_expr(dfp,"dpri->cache = free_BaseMatrix(dpri->cache)");
  closebrace(dfp);
  closebrace(dfp);

  expr(dfp,"if( out->basematrix == NULL )");
  startbrace(dfp);
  expr(dfp,"if( (out->basematrix = BaseMatrix_alloc_matrix_and_specials((out->lenj+%d)*%d,(out->leni+%d),%d,out->lenj+%d)) == NULL)",gm->window_j,gm->len,gm->window_i,gm->spec_len,gm->window_j);
  startbrace(dfp);
  expr(dfp,"warn(\"Explicit matrix %s cannot be allocated, (asking for %%d by %%d main cells)\",out->leni,out->lenj);",gm->name);
  expr(dfp,"free_%s(out)",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);
  closebrace(dfp);

  expr(dfp,"if( dpri->should_cache == TRUE && dpri->cache == NULL)");
  hang_expr(dfp,"dpri->cache = hard_link_BaseMatrix(out->basematrix)");

  expr(dfp,"out->basematrix->type = BASEMATRIX_TYPE_EXPLICIT;");
  expr(dfp,"init_%s(out)",gm->name);

  expr(dfp,"return out");
  
  close_function(dfp);
  
  add_break(dfp);
  
}



/* Function:  write_safe_alloc_function(dfp,gm)
 *
 * Descrip:    produces the allocate_xxx_only function,
 *             which allocates the matrix structure, checks
 *             resources which it can check, but does NOT 
 *             allocate basematrix area
 *
 *             This function will be called by allocate_Expl_xxxx 
 *             and allocate_Small_xxxx etc.
 *
 *
 * Arg:        dfp [UNKN ] dynmaite file pointer to func/head [DYNFILE *]
 * Arg:         gm [UNKN ] generic matrix structure [GenericMatrix *]
 *
 */
# line 357 "dynafunc.dy"
void write_safe_alloc_function(DYNFILE * dfp,GenericMatrix * gm)
{
  int i;
  FuncInfo * fi;
  char * arg_str;
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"allocate_%s_only",gm->name);
  
  add_line_to_Ftext(fi->ft,"This function only allocates the %s structure",gm->name);
  add_line_to_Ftext(fi->ft,"checks types where possible and determines leni and lenj");
  add_line_to_Ftext(fi->ft,"The basematrix area is delt with elsewhere");

  arg_str = get_argstr_GenericMatrix(gm);
  add_args_GenericMatrix_FuncInfo(fi,gm);


  start_function_FuncInfo(fi,dfp,"%s * allocate_%s_only(%s)",gm->name,gm->name,arg_str);

  /*** clean up ***/
  ckfree(arg_str);


  expr(dfp,"%s * out",gm->name); add_break(dfp);

  expr(dfp,"if((out= %s_alloc()) == NULL)",gm->name);
  startbrace(dfp);
  warn_expr(dfp,"Allocation of basic %s structure failed...",gm->name);
  expr(dfp,"return NULL;");
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"out->%s = %s",gm->query->name,gm->query->name);
  expr(dfp,"out->%s = %s",gm->target->name,gm->target->name);

  for(i=0;i<gm->res_len;i++) {
    expr(dfp,"out->%s = %s",gm->resource[i]->name,gm->resource[i]->name);
  }

  expr(dfp,"out->leni = %s->%s",gm->query->name,gm->query_len);
  expr(dfp,"out->lenj = %s->%s",gm->target->name,gm->target_len);

  expr(dfp,"return out");

  close_function(dfp);
  add_break(dfp);
}


/* Function:  add_args_GenericMatrix_FuncInfo(fi,gm)
 *
 * Descrip:    Information partner to /get_argstr_GenericMatrix.
 *             Loads up the arglist information for the GenericMatrix,
 *             ie query-type query-name "what information" etc.
 *
 *
 *
 * Arg:        fi [WRITE] FuncInfo structure to add ArgInfo to  [FuncInfo *]
 * Arg:        gm [READ ] generic matrix structure to read from [const GenericMatrix *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
# line 415 "dynafunc.dy"
boolean add_args_GenericMatrix_FuncInfo(FuncInfo * fi,const GenericMatrix * gm)
{
  int i;
  ArgInfo * ai;

  /** ArgInfo_in_FuncInfo_from_varstr automatically adds ai to fi **/
  
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,gm->query->name);
  ai->desc=stringalloc("query data structure");

  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,gm->target->name);
  ai->desc=stringalloc("target data structure");


  for(i=0;i<gm->res_len;i++) {
    ai =  ArgInfo_in_FuncInfo_from_varstr(fi,gm->resource[i]->name);
    ai->desc=stringalloc("Resource");
  }

  return TRUE;
}

/* Function:  get_chainstr_GenericMatrix(gm)
 *
 * Descrip:    makes an the argument calling string which
 *             is compatible with the arg_str from
 *             /get_argstr_GenericMatrix
 *
 *             eg "query,target,comp_mat"
 *
 *
 * Arg:        gm [READ ] structure holding generic matrix [const GenericMatrix *]
 *
 * Return [UNKN ]  allocated string (must free) with chained-args [char *]
 *
 */
# line 447 "dynafunc.dy"
char * get_chainstr_GenericMatrix(const GenericMatrix * gm)
{
  char buffer[MAXLINE];
  int i;
  
  sprintf(buffer,"%s, %s ",gm->query->name,gm->target->name);
  
  for(i=0;i<gm->res_len;i++) {
    strcat(buffer,", ");
    strcat(buffer,gm->resource[i]->name);
  }

  return stringalloc(buffer);

}
    
/* Function:  get_argstr_GenericMatrix(gm)
 *
 * Descrip:    makes the argument list for a generic matrix alloc, ie
 *             query-type query-name,target-type target-name etc
 *
 *             To load up info, use /add_arg_GenericMatrix_FuncInfo
 *             To chain up along function calls, use /get_chainstr_GenericMatrix
 *
 *
 * Arg:        gm [READ ] structure holding generic matrix [const GenericMatrix *]
 *
 * Return [UNKN ]  allocated string (must free) with args [char *]
 *
 */
# line 473 "dynafunc.dy"
char * get_argstr_GenericMatrix(const GenericMatrix * gm)
{
  char buffer[MAXLINE];
  int i;

  sprintf(buffer,"%s %s,%s %s ",gm->query->element_type,gm->query->name,gm->target->element_type,gm->target->name);

  for(i=0;i<gm->res_len;i++) {
    strcat(buffer,",");
    strcat(buffer,gm->resource[i]->element_type);
    strcat(buffer," ");
    strcat(buffer,gm->resource[i]->name);
  }

  return stringalloc(buffer);
}
 
# line 490 "dynafunc.dy"
void write_dotter_dump(DYNFILE * dfp,GenericMatrix * gm)
{

  start_function(dfp,"void dump_dotter_%s(%s * mat,FILE * ofp)",gm->name,gm->name);

  expr(dfp,"register int i");
  expr(dfp,"register int j");
  expr(dfp,"register int k");
  expr(dfp,"int hlen");
  expr(dfp,"int vlen");
  expr(dfp,"char form = 1;");
  expr(dfp,"int zoom = 1;");
  expr(dfp,"register int max = (-1000000)");

/***
  expr(dfp,"fprintf(ofp,\"fileformat 1\\nzoomfactor 10\\nhorizontal_len %%d\\nvertical_len %%d\\n\",mat->%s->%s + (4-((mat->%s->%s)%%4)),mat->%s->%s + (4-((mat->%s->%s)%%4)));",
gm->query->name,gm->query_len,gm->query->name,gm->query_len,gm->target->name,gm->target_len,gm->target->name,gm->target_len);
***/


  expr(dfp,"hlen = mat->%s->%s",gm->query->name,gm->query_len);
  expr(dfp,"if( hlen%%4 != 0 ) ");
  hang_expr(dfp,"hlen += 4 - hlen%%4;");

  expr(dfp,"vlen = mat->%s->%s",gm->target->name,gm->target_len);
  expr(dfp,"if( vlen%%4 != 0 ) ");
  hang_expr(dfp,"vlen += 4 - hlen%%4;");
  
  expr(dfp,"fwrite(&form,sizeof(char),1,ofp)");
  expr(dfp,"fwrite(&zoom,sizeof(int),1,ofp)");
  expr(dfp,"fwrite(&hlen,sizeof(int),1,ofp)");
  expr(dfp,"fwrite(&vlen,sizeof(int),1,ofp)");
  

  expr(dfp,"for(i=0;i<mat->%s->%s;i++)",gm->query->name,gm->query_len);
  startbrace(dfp);
  expr(dfp,"for(j=0;j<mat->%s->%s;j++)",gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"if( %s_EXPL_MATRIX(mat,i,j,1) > max )",gm->name);
  expr(dfp,"max = %s_EXPL_MATRIX(mat,i,j,1)",gm->name);
  closebrace(dfp);
  closebrace(dfp);

  
  expr(dfp,"for(i=0;i<mat->%s->%s;i++)",gm->query->name,gm->query_len);
  startbrace(dfp);
  expr(dfp,"for(j=0;j<mat->%s->%s;j++)",gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"fputc( (char) (%s_EXPL_MATRIX(mat,i,j,1)*256 / max),ofp )",gm->name);
  closebrace(dfp);
  closebrace(dfp);


  close_function(dfp);
}
  

# line 547 "dynafunc.dy"
void write_memory_macros(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
	
  for(i=0;i<gm->len;i++)
    macro(dfp,"#define %s %d",gm->state[i]->name,i);
  add_break(dfp);
  for(i=0;i<gm->spec_len;i++)
    macro(dfp,"#define %s %d",gm->special[i]->name,i);
  
  add_break(dfp);
  macro(dfp,"#define %s_EXPL_MATRIX(this_matrix,i,j,STATE) this_matrix->basematrix->matrix[((j+%d)*%d)+STATE][i+%d]",gm->name,gm->window_j,gm->len,gm->window_i);
  macro(dfp,"#define %s_EXPL_SPECIAL(matrix,i,j,STATE) matrix->basematrix->specmatrix[STATE][j+%d]",gm->name,gm->window_j);
  macro(dfp,"#define %s_READ_OFF_ERROR -%d\n",gm->name,gm->window_i+gm->window_j+1);	
  add_break(dfp);
}

# line 564 "dynafunc.dy"
void write_search_macros(DYNFILE * dfp,const GenericMatrix * gm,DPImplementation * dpi)
{

  if(gm->qtype != NULL && gm->qtype->maxlen != NULL) {
    macro(dfp,"#define %s_VSMALL_MATRIX(mat,i,j,STATE) internal_matrix[(j+%d)%%%d][((i+%d)*%d)+STATE]",gm->name,gm->window_j+1,gm->window_j+1,gm->window_i,gm->len);
    macro(dfp,"#define %s_VSMALL_SPECIAL(mat,i,j,STATE) internal_specials[(j+%d)%%%d][STATE]",gm->name,gm->window_j+1,gm->window_j+1);
    
  } else {
    if( dpi->largemem == TRUE ) {
      macro(dfp,"#define %s_VSMALL_MATRIX(mat,i,j,STATE) internal_pointer_db[((j+%d)%%%d * (mat->leni+%d) * %d) +((i+%d)*%d)+STATE]",gm->name,gm->window_j+1,gm->window_j+1,gm->window_i,gm->len,gm->window_i,gm->len);
      macro(dfp,"#define %s_VSMALL_SPECIAL(mat,i,j,STATE) internal_special_db[((j+%d)%%%d * %d)+ STATE]",gm->name,gm->window_j+1,gm->window_j+1,gm->spec_len);
    } else {
      macro(dfp,"#define %s_VSMALL_MATRIX(mat,i,j,STATE) mat->basematrix->matrix[(j+%d)%%%d][((i+%d)*%d)+STATE]",gm->name,gm->window_j+1,gm->window_j+1,gm->window_i,gm->len);
      macro(dfp,"#define %s_VSMALL_SPECIAL(mat,i,j,STATE) mat->basematrix->specmatrix[(j+%d)%%%d][STATE]",gm->name,gm->window_j+1,gm->window_j+1);
    }
  }

  add_break(dfp);
}

# line 584 "dynafunc.dy"
void write_expl_access_funcs(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"%s_explicit_access_main",gm->name);
  start_function_FuncInfo(fi,dfp,"int %s_explicit_access_main(%s * mat,int i,int j,int state)",gm->name,gm->name);
  expr(dfp,"return %s_EXPL_MATRIX(mat,i,j,state);",gm->name);
  close_function(dfp);
  add_break(dfp);

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"%s_explicit_access_special",gm->name);
  start_function_FuncInfo(fi,dfp,"int %s_explicit_access_special(%s * mat,int i,int j,int state)",gm->name,gm->name);
  expr(dfp,"return %s_EXPL_SPECIAL(mat,i,j,state);",gm->name);
  close_function(dfp);
  add_break(dfp);
}
  

# line 602 "dynafunc.dy"
void write_expl_read_func(DYNFILE * dfp,GenericMatrix * gm)
{

  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"PackAln_read_Expl_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Reads off PackAln from explicit matrix structure",gm->name);

  start_function_FuncInfo(fi,dfp,"PackAln * PackAln_read_Expl_%s(%s * mat)",gm->name,gm->name);
  expr(dfp,"%s_access_func_holder holder",gm->name);
  add_break(dfp);
  expr(dfp,"holder.access_main    = %s_explicit_access_main;",gm->name);
  expr(dfp,"holder.access_special = %s_explicit_access_special;",gm->name);

  expr(dfp,"return PackAln_read_generic_%s(mat,holder);",gm->name);
 
  close_function(dfp);
  add_break(dfp);
}


# line 623 "dynafunc.dy"
void write_basic_read_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
  register int st;
  FuncInfo * fi;

  for(i=0;i<gm->spec_len;i++)
    if( gm->special[i]->is_start == TRUE )
      break;
  
  if( i >= gm->spec_len )
    log_full_error(FATAL,0,"Cannot make matrix with no !start state (must be a special)");
  
  st = i;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"PackAln_read_generic_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Reads off PackAln from explicit matrix structure",gm->name);

  start_function_FuncInfo(fi,dfp,"PackAln * PackAln_read_generic_%s(%s * mat,%s_access_func_holder h)",gm->name,gm->name,gm->name);
  
  expr(dfp,"register PackAln * out;");
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int state;");
  expr(dfp,"int cellscore = (-1);");
  expr(dfp,"boolean isspecial");
  
  expr(dfp,"PackAlnUnit * pau = NULL;");
  expr(dfp,"PackAlnUnit * prev = NULL;");
  
  add_break(dfp);
  
  expr(dfp,"assert(mat)");
  expr(dfp,"assert(h.access_main)");
  expr(dfp,"assert(h.access_special)");
  
  add_break(dfp);
  expr(dfp,"out = PackAln_alloc_std()");
  expr(dfp,"if( out == NULL )");
  hang_expr(dfp,"return NULL;");
  
  add_break(dfp);
  
  /*** Find the end of the matrix ***/
  
  expr(dfp,"out->score =  find_end_%s(mat,&i,&j,&state,&isspecial,h);",gm->name);
  
  add_break(dfp);
  
  add_block_comment(dfp,"Add final end transition (at the moment we have not got the score!");
  expr(dfp,"if( (pau= PackAlnUnit_alloc()) == NULL  || add_PackAln(out,pau) == FALSE )");
  startbrace(dfp);
  expr(dfp,"warn(\"Failed the first PackAlnUnit alloc, %%d length of Alignment in %s_basic_read, returning a mess.(Sorry!)\",out->len);",gm->name);
  expr(dfp,"return out;");
  closebrace(dfp);
  
  add_break(dfp);

  /** put in numbers from block **/
  add_block_comment(dfp,"Put in positions for end trans. Remember that coordinates in C style");

  expr(dfp,"pau->i = i;");
  expr(dfp,"pau->j = j;");
  expr(dfp,"if( isspecial != TRUE) ");
  hang_expr(dfp,"pau->state = state;");
  expr(dfp,"else pau->state = state + %d",gm->len);
/***  expr(dfp,"pau->score = cellscore"); ***/
  expr(dfp,"prev=pau;");

  /**** do the loop ****/
  
  /*** expr(dfp,"start_reporting(\"Alignment [Score %%d]:\",out->score);"); ***/
  
  expr(dfp,"while( state != %s || isspecial != TRUE)",gm->special[st]->name);
  startbrace_tag(dfp,"while state != START");
  
  
  add_break(dfp);
  expr(dfp,"if( isspecial == TRUE )");		
  hang_expr(dfp,"max_calc_special_%s(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore,h)",gm->name);
  expr(dfp,"else");
  hang_expr(dfp,"max_calc_%s(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore,h)",gm->name);	

  
  /*** check we haven't boobied ***/
  
  expr(dfp,"if(i == %s_READ_OFF_ERROR || j == %s_READ_OFF_ERROR || state == %s_READ_OFF_ERROR )",gm->name,gm->name,gm->name);
  
  startbrace(dfp);
  expr(dfp,"warn(\"Problem - hit bad read off system, exiting now\");");
  expr(dfp,"break;");
  closebrace(dfp);

  /** allocate position **/

  expr(dfp,"if( (pau= PackAlnUnit_alloc()) == NULL  || add_PackAln(out,pau) == FALSE )");
  startbrace(dfp);
  expr(dfp,"warn(\"Failed a PackAlnUnit alloc, %%d length of Alignment in %s_basic_read, returning partial alignment\",out->len);",gm->name);
  expr(dfp,"break;");
  closebrace(dfp);
  
  add_break(dfp);

  /** put in numbers from block **/
  add_block_comment(dfp,"Put in positions for block. Remember that coordinates in C style");

  expr(dfp,"pau->i = i;");
  expr(dfp,"pau->j = j;");
  expr(dfp,"if( isspecial != TRUE) ");
  hang_expr(dfp,"pau->state = state;");
  expr(dfp,"else pau->state = state + %d",gm->len);
  expr(dfp,"prev->score = cellscore");
  expr(dfp,"prev = pau;");

  closebrace(dfp);

  /**** Add in alignment flips (or do I need to do that later!) ****/
  add_break(dfp);
  
  /*** expr(dfp,"stop_reporting()"); ***/

  expr(dfp,"invert_PackAln(out);");
  expr(dfp,"return out;");

  close_function(dfp);
  add_break(dfp);
}

# line 751 "dynafunc.dy"
void write_pal_to_ars_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"AlnRangeSet_from_PackAln_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Converts a PackAln (full alignment) to start/stop range");
  add_line_to_Ftext(fi->ft,"The point being that you may have the PackAln and someone wants a range",gm->name);

  start_function_FuncInfo(fi,dfp,"AlnRangeSet * AlnRangeSet_from_PackAln_%s(PackAln * pal)",gm->name);
  expr(dfp,"int unit;");
  expr(dfp,"int cum_score = 0;");
  expr(dfp,"AlnRangeSet * out;");
  expr(dfp,"AlnRange * temp;");

  add_break(dfp);

  expr(dfp,"out = AlnRangeSet_alloc_std();");

  expr(dfp,"for(unit = 0;unit < pal->len;unit++)");
  startbrace_tag(dfp,"while there are more units");
  expr(dfp,"cum_score += pal->pau[unit]->score;");
  expr(dfp,"if( pal->pau[unit]->state < %d)",gm->len);
  startbrace(dfp);
  add_block_comment(dfp,"It is out of the specials");
  expr(dfp,"temp = AlnRange_alloc();");
  expr(dfp,"temp->starti = pal->pau[unit]->i;");
  expr(dfp,"temp->startj = pal->pau[unit]->j;");
  expr(dfp,"temp->startstate = pal->pau[unit]->state;");
  expr(dfp,"temp->startscore = cum_score;");

  expr(dfp,"for(;unit < pal->len && pal->pau[unit]->state < %d;unit++)",gm->len);
  hang_expr(dfp,"cum_score += pal->pau[unit]->score;");
  expr(dfp,"temp->stopi = pal->pau[unit-1]->i;");
  expr(dfp,"temp->stopj = pal->pau[unit-1]->j;");
  expr(dfp,"temp->stopstate = pal->pau[unit-1]->state;");
  expr(dfp,"temp->stopscore = cum_score;");
  expr(dfp,"add_AlnRangeSet(out,temp)");
  closebrace(dfp);
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"return out");

  close_function(dfp);

  add_break(dfp);

}


# line 802 "dynafunc.dy"
void write_alncconvert_make_func(DYNFILE * dfp,GenericMatrix * gm)
{
  int i;
  int j;
  LabelMaster * lm;
  char * qs;
  char * ts;
  int qlabel;
  int tlabel;
  FuncInfo * fi;



  lm = LabelMaster_from_GenericMatrix(gm);
  qs =  query_quoted_string_from_LabelMaster(lm);
  ts = target_quoted_string_from_LabelMaster(lm);

  expr(dfp," static char * query_label[] = { %s };",qs);
  expr(dfp," static char * target_label[] = { %s };",ts);

  ckfree(qs);
  ckfree(ts);

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"AlnConvertSet_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"AlnConvertSet * AlnConvertSet_%s(void)",gm->name);
  expr(dfp,"AlnConvertUnit * acu;");
  expr(dfp,"AlnConvertSet  * out;");
  add_break(dfp);
  expr(dfp,"out = AlnConvertSet_alloc_std();");
  add_break(dfp);

  for(i=0;i<gm->len;i++) {
    for(j=0;j<gm->state[i]->len;j++) {
      qlabel = index_for_query_label(gm->state[i]->source[j]->query_label,lm);
      if( qlabel == (-1) ) {
	warn("Query label [%s] unfound. Consider this a bad compiler bug!",gm->state[i]->source[j]->query_label);
	qlabel = 0;
      }
      tlabel = index_for_target_label(gm->state[i]->source[j]->target_label,lm);
      if( tlabel == (-1) ) {
	warn("Query label [%s] unfound. Consider this a bad compiler bug!",gm->state[i]->source[j]->target_label);
	tlabel = 0;
      }

      expr(dfp,"acu = AlnConvertUnit_alloc();");
      expr(dfp,"add_AlnConvertSet(out,acu);");
      if( gm->state[i]->source[j]->isspecial == FALSE ) {
	expr(dfp,"acu->state1 = %s;",gm->state[i]->source[j]->state_source);
      }
      else {
	expr(dfp,"acu->state1 = %s + %d;",gm->state[i]->source[j]->state_source,gm->len);
	expr(dfp,"acu->is_from_special = TRUE;");
      }

      expr(dfp,"acu->state2 = %s",gm->state[i]->name);
      
      if( gm->state[i]->source[j]->isspecial == FALSE )
	expr(dfp,"acu->offi = %d;",gm->state[i]->source[j]->offi);
      else expr(dfp,"acu->offi = (-1);");
      expr(dfp,"acu->offj = %d;",gm->state[i]->source[j]->offj);
      expr(dfp,"acu->label1 = query_label[%d];",qlabel);
      expr(dfp,"acu->label2 = target_label[%d];",tlabel);
    }
  }

  for(i=0;i<gm->spec_len;i++) {
    for(j=0;j<gm->special[i]->len;j++) {
      qlabel = index_for_query_label(gm->special[i]->source[j]->query_label,lm);
      if( qlabel == (-1) ) {
	warn("Query label [%s] unfound. Consider this a bad compiler bug!",gm->special[i]->source[j]->query_label);
	qlabel = 0;
      }
      tlabel = index_for_target_label(gm->special[i]->source[j]->target_label,lm);
      if( tlabel == (-1) ) {
	warn("Query label [%s] unfound. Consider this a bad compiler bug!",gm->special[i]->source[j]->target_label);
	tlabel = 0;
      }

      expr(dfp,"acu = AlnConvertUnit_alloc();");
      expr(dfp,"add_AlnConvertSet(out,acu);");
      if( gm->special[i]->source[j]->isspecial == FALSE )
	expr(dfp,"acu->state1 = %s;",gm->special[i]->source[j]->state_source);
      else expr(dfp,"acu->state1 = %s + %d;",gm->special[i]->source[j]->state_source,gm->len);
      expr(dfp,"acu->state2 = %s + %d",gm->special[i]->name,gm->len);
      
      expr(dfp,"acu->offi = (-1);");
      expr(dfp,"acu->offj = %d;",gm->special[i]->source[j]->offj);
      expr(dfp,"acu->label1 = query_label[%d];",qlabel);
      expr(dfp,"acu->label2 = target_label[%d];",tlabel);
    }
  }

  for(i=0;i<gm->cal_len;i++) {
    expr(dfp,"add_collapse_label_AlnConvertSet(out,\"%s\",\"%s\");",gm->cal[i]->query,gm->cal[i]->target);
  }

  

  expr(dfp,"return out;");
  close_function(dfp);
  add_break(dfp);

}

# line 907 "dynafunc.dy"
void write_aln_conversion_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"convert_PackAln_to_AlnBlock_%s",gm->name);
  add_line_to_Ftext(fi->ft,"Converts a path alignment to a label alignment");
  add_line_to_Ftext(fi->ft,"The label alignment is probably much more useful than the path",gm->name);

  start_function_FuncInfo(fi,dfp,"AlnBlock * convert_PackAln_to_AlnBlock_%s(PackAln * pal)",gm->name,gm->name);
  expr(dfp,"AlnConvertSet * acs;");
  expr(dfp,"AlnBlock * alb;");
  add_break(dfp);
  expr(dfp,"acs = AlnConvertSet_%s();",gm->name);
  expr(dfp,"alb = AlnBlock_from_PackAln(acs,pal);");
  expr(dfp,"free_AlnConvertSet(acs);");
  expr(dfp,"return alb;");
  close_function(dfp);

  add_break(dfp);
}


/* Function:  init_matrix_func(dfp,gm)
 *
 * Descrip:    init explicit matrix
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 932 "dynafunc.dy"
void init_matrix_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  ArgInfo * ai;
  register int i;

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"init_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function initates %s matrix when in explicit mode",gm->name);
  add_line_to_Ftext(fi->ft,"Called in /allocate_Expl_%s",gm->name);


  /** ArgInfo_in_FuncInfo_from_varstr automatically adds ai to fi **/
  
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"mat");
  ai->desc=stringallocf("%s which contains explicit basematrix memory",gm->name);

  start_function_FuncInfo(fi,dfp,"void init_%s(%s * mat)",gm->name,gm->name);
	
  expr(dfp,"register int i;");	
  expr(dfp,"register int j;");

  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT)");
  startbrace(dfp);
  warn_expr(dfp,"Cannot iniate matrix, is not an explicit memory type and you have assummed that");
  expr(dfp,"return");
  closebrace(dfp);

  add_break(dfp);

  expr(dfp,"for(i= (-%d);i<mat->%s->%s;i++)",gm->window_i,gm->query->name,gm->query_len);
  startbrace(dfp);
  expr(dfp,"for(j= (-%d);j<%d;j++)",gm->window_j,gm->window_j+1);	
  startbrace(dfp);
  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_EXPL_MATRIX(mat,i,j,%s) = %s;",gm->name,gm->state[i]->name,gm->state[i]->def_score);
  }
  closebrace(dfp);
  closebrace(dfp);
  
  expr(dfp,"for(j= (-%d);j<mat->%s->%s;j++)",gm->window_j,gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"for(i= (-%d);i<%d;i++)",gm->window_i,gm->window_i+1);
  startbrace(dfp);
  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_EXPL_MATRIX(mat,i,j,%s) = %s;",gm->name,gm->state[i]->name,gm->state[i]->def_score);
  }
  closebrace(dfp);
  for(i=0;i<gm->spec_len;i++) {
    expr(dfp,"%s_EXPL_SPECIAL(mat,i,j,%s) = %s;",gm->name,gm->special[i]->name,gm->special[i]->def_score);
  }
  closebrace(dfp);
  
  
  expr(dfp,"return");
  close_function(dfp);
  
  add_break(dfp);
   
}
       

# line 993 "dynafunc.dy"
void find_end_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
  FuncInfo * fi;


  
  for(i=0;i<gm->spec_len;i++)
    if( gm->special[i]->is_end == TRUE)
      break;

  if( i >= gm->spec_len ) {
    log_full_error(WARNING,0,"Cannot build find end function due to lack of end state");
    return;
  }
  
  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"find_end_%s",gm->name);
  start_function_FuncInfo(fi,dfp,"int find_end_%s(%s * mat,int * ri,int * rj,int * state,boolean * isspecial,%s_access_func_holder h)",gm->name,gm->name,gm->name);
	
  expr(dfp,"int j");
  expr(dfp,"int max;");
  expr(dfp,"int maxj");
  expr(dfp,"int temp");
  
  add_break(dfp);
  
  expr(dfp,"max = (*h.access_special)(mat,0,mat->%s->%s-1,%s);",gm->target->name,gm->target_len,gm->special[i]->name);
  expr(dfp,"maxj = mat->%s->%s-1",gm->target->name,gm->target_len);
  expr(dfp,"for(j= mat->%s->%s-2 ;j >= 0 ;j--)",gm->target->name,gm->target_len);
  
  startbrace(dfp);
  expr(dfp,"if( (temp =(*h.access_special)(mat,0,j,%s)) > max )",gm->special[i]->name);
  startbrace(dfp);
  expr(dfp,"max = temp;");
  expr(dfp,"maxj = j;");
  closebrace(dfp);
  closebrace(dfp);
  
  add_break(dfp);
  
  expr(dfp,"if( ri != NULL)");
  hang_expr(dfp," *ri = 0; ");
  expr(dfp,"if( rj != NULL)");
  hang_expr(dfp," *rj = maxj; ");
  expr(dfp,"if( state != NULL)");
  hang_expr(dfp," *state = %s;",gm->special[i]->name);
  expr(dfp,"if( isspecial != NULL)");
  hang_expr(dfp," *isspecial = TRUE;");
  

  add_break(dfp);
  
  expr(dfp,"return max");
  
  close_function(dfp);
  
  add_break(dfp);
  
}
	

# line 1054 "dynafunc.dy"
void debug_func(DYNFILE * dfp,GenericMatrix * gm)
{
  register int i;
  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"%s_debug_show_matrix",gm->name);
  
  start_function_FuncInfo(fi,dfp,"void %s_debug_show_matrix(%s * mat,int starti,int stopi,int startj,int stopj,FILE * ofp)",gm->name,gm->name);
  
  expr(dfp,"register int i;");
  expr(dfp,"register int j;");
  
  add_break(dfp);

  expr(dfp,"for(i=starti;i<stopi && i < mat->%s->%s;i++)",gm->query->name,gm->query_len);
  startbrace(dfp);
  expr(dfp,"for(j=startj;j<stopj && j < mat->%s->%s;j++)",gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"fprintf(ofp,\"Cell [%%d - %%d]\\n\",i,j)");
  for(i=0;i<gm->len;i++)
    expr(dfp,"fprintf(ofp,\"State %s %%d\\n\",%s_EXPL_MATRIX(mat,i,j,%s));",gm->state[i]->name,gm->name,gm->state[i]->name);
  expr(dfp,"fprintf(ofp,\"\\n\\n\");");
  closebrace(dfp);
  closebrace(dfp);
	add_break(dfp);
  close_function(dfp);
  add_break(dfp);
  
  return;
}
	
# line 1085 "dynafunc.dy"
void write_special_max_calc_func_debug(DYNFILE * dfp,GenericMatrix * gm,int debug)
{
  register int i;
  register int j;
  FuncInfo * fi;
  char * matrix_tag = "EXPL_MATRIX";
  char * special_tag = "EXPL_SPECIAL";

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"max_calc_special_%s",gm->name);

  
  start_function_FuncInfo(fi,dfp,"int max_calc_special_%s(%s * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore,%s_access_func_holder h)",gm->name,gm->name,gm->name);
  
  expr(dfp,"register int temp;");
  expr(dfp,"register int cscore;");
  
  add_break(dfp);
  
  expr(dfp,"*reti = (*retj) = (*retstate) = %s_READ_OFF_ERROR;",gm->name);
  
  add_break(dfp);
  
  expr(dfp,"if( j < 0 || j > mat->%s->%s)",gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"warn(\"In %s matrix special read off - out of bounds on matrix [j is %%d in special]\",j);",gm->name);
  expr(dfp,"return -1");
  closebrace(dfp);
  
  add_break(dfp);
  
  expr(dfp,"cscore = (*h.access_special)(mat,i,j,state);");
  
  if( debug)
    expr(dfp,"fprintf(stderr,\"Looking for score %%d\\n\",cscore);");
  
  expr(dfp,"switch(state)");
  startbrace_tag(dfp,"switch on special states");
  
  for(i=0;i<gm->spec_len;i++) {
    auto CellState * cell;
    cell = gm->special[i];
    expr(dfp,"case %s : ",cell->name);
    startcase(dfp);


    /*** ok, this is a problem. probably should do special to special first, ***/
    /*** the others. Hmmm.,.. ***/


    for(j=cell->len-1;j>=0;j--) {
      
      if( cell->source[j]->isspecial == FALSE ) {
	add_block_comment(dfp,"source %s is from main matrix",cell->source[j]->state_source);
	expr(dfp,"for(i= mat->%s->%s-1;i >= 0 ;i--)",gm->query->name,gm->query_len);
	startbrace_tag(dfp,"for i >= 0");
      
	expr(dfp,"temp = cscore - (%s) - (%s)",cell->source[j]->calc_expr, 
	     cell->calc_expr == NULL ? "0" : cell->calc_expr);
      
	if( debug )
	  expr(dfp,"fprintf(stderr,\"Trying %%d for case %s\\n\",temp)",cell->source[j]->state_source); 

	expr(dfp,"if( temp == (*h.access_main)(mat,i - %d,j - %d,%s) )",cell->source[j]->offi,
	       cell->source[j]->offj,cell->source[j]->state_source);
  
	startbrace(dfp);
	expr(dfp,"*reti = i - %d;",cell->source[j]->offi);
	expr(dfp,"*retj = j - %d;",cell->source[j]->offj);
	expr(dfp,"*retstate = %s;",cell->source[j]->state_source);
	
	if( cell->source[j]->isspecial == TRUE)
	  expr(dfp,"*retspecial = TRUE;");
	else	expr(dfp,"*retspecial = FALSE;");


	/*** if they want the score, calc the difference in the number and give it back ***/
	expr(dfp,"if( cellscore != NULL)");
	startbrace(dfp);
	expr(dfp,"*cellscore = cscore - (*h.access_main)(mat,i-%d,j-%d,%s)",cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source); 
	closebrace(dfp);
	
	expr(dfp,"return (*h.access_main)(mat,i - %d,j - %d,%s) ",cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source); 
	closebrace(dfp);
	closebrace(dfp); /** this closes the for brace ***/
      }
      

      else {
	add_block_comment(dfp,"source %s is a special",cell->source[j]->state_source);

	expr(dfp,"temp = cscore - (%s) - (%s)",cell->source[j]->calc_expr,
	     cell->calc_expr == NULL ? "0" : cell->calc_expr);
      
	if( debug )
	  expr(dfp,"fprintf(stderr,\"Trying %%d for case %s\\n\",temp)",cell->source[j]->state_source); 

	expr(dfp,"if( temp == (*h.access_special)(mat,i - %d,j - %d,%s) )",cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source);
          

	startbrace(dfp);
	expr(dfp,"*reti = i - %d;",cell->source[j]->offi);
	expr(dfp,"*retj = j - %d;",cell->source[j]->offj);
	expr(dfp,"*retstate = %s;",cell->source[j]->state_source);
	
	if( cell->source[j]->isspecial == TRUE)
	  expr(dfp,"*retspecial = TRUE;");
	else	expr(dfp,"*retspecial = FALSE;");

	/*** if they want the score, calc the difference in the number and give it back ***/
	expr(dfp,"if( cellscore != NULL)");
	startbrace(dfp);
	expr(dfp,"*cellscore = cscore - (*h.access_special)(mat,i-%d,j-%d,%s)",cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source); 
	closebrace(dfp);
      
	expr(dfp,"return (*h.access_special)(mat,i - %d,j - %d,%s) ",cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source); 
	closebrace(dfp);
      
      }
    }
    closecase(dfp);
  }
  
  expr(dfp,"default:");
  startcase(dfp);
  expr(dfp,"warn(\"Major problem (!) - in %s read off, position %%d,%%d state %%d no source found  dropped into default on source switch!\",i,j,state);",gm->name);
  expr(dfp,"return (-1);");
  closecase(dfp);
  
  closebrace(dfp);
  
  close_function(dfp);
  
  add_break(dfp);
}

# line 1225 "dynafunc.dy"
void write_special_max_calc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  write_special_max_calc_func_debug(dfp,gm,0);
}

 /****
   This function requires alot of stuff assummed.

   int * reti, int *retj and *retstate *retspecial as parameters which will be set 
   by this block.

   This block is really to facillate different underlying memory structures
   which can be used. 

   Please see functions write_max_calc_func and write_hidden_max_func (dynashadow.dy)
   to see how it is used

   ***/

# line 1244 "dynafunc.dy"
void write_max_calc_block(DYNFILE * dfp,GenericMatrix * gm,char * matrix_tag,char * special_tag,boolean use_special,boolean use_holder)
{
  register int i;
  register int j;
  char matrix_access[128];
  char special_access[128];


  if( use_holder == TRUE ) {
    sprintf(matrix_access,"(*h.access_main)");
    sprintf(special_access,"(*h.access_special)");
  } else {
    sprintf(matrix_access,"%s_%s",gm->name,matrix_tag);
    sprintf(special_access,"%s_%s",gm->name,special_tag);
  }


  add_break(dfp);
  add_block_comment(dfp,"Then you have to select the correct switch statement to figure out the readoff     ");
  add_block_comment(dfp,"Somewhat odd - reverse the order of calculation and return as soon as it is correct");
  
  
  expr(dfp,"cscore = %s(mat,i,j,state);",matrix_access);
  expr(dfp,"switch(state)");
  
  startbrace_tag(dfp,"Switch state ");
  for(i=0;i<gm->len;i++) {
    auto CellState * cell;
    cell = gm->state[i];
    expr(dfp,"case %s : ",cell->name);
    startcase(dfp);
    for(j=cell->len-1;j>=0;j--) {
      if( use_special == FALSE && cell->source[j]->isspecial == TRUE ) {
	add_block_comment(dfp,"Not allowing special sources.. skipping %s",cell->source[j]->state_source);
	continue;
      }

      if( cell->source[j]->position != SOURCE_POS_ALL) {
	add_block_comment(dfp,"Has restricted position");
	expr(dfp,"if( %s )",source_allowed_statement(cell->source[j]->position,cell->source[j]->offi,cell->source[j]->offj));
	startbrace(dfp);
      }

      expr(dfp,"temp = cscore - (%s) -  (%s);",cell->source[j]->calc_expr, 
	   cell->calc_expr == NULL ? "0" : cell->calc_expr);
      
      if( cell->source[j]->isspecial == TRUE)
	expr(dfp,"if( temp == %s(mat,i - %d,j - %d,%s) )",special_access,cell->source[j]->offi,
	     cell->source[j]->offj,cell->source[j]->state_source);
      else	expr(dfp,"if( temp == %s(mat,i - %d,j - %d,%s) )",matrix_access,cell->source[j]->offi,
		     cell->source[j]->offj,cell->source[j]->state_source);
      
      startbrace(dfp);
      expr(dfp,"*reti = i - %d;",cell->source[j]->offi);
      expr(dfp,"*retj = j - %d;",cell->source[j]->offj);
      expr(dfp,"*retstate = %s;",cell->source[j]->state_source);
      if( cell->source[j]->isspecial == TRUE)
	expr(dfp,"*retspecial = TRUE;");
      else expr(dfp,"*retspecial = FALSE;");
      
      /*** if they want the score, calc the difference in the number and give it back ***/
      expr(dfp,"if( cellscore != NULL)");
      startbrace(dfp);

      
      expr(dfp,"*cellscore = cscore - %s(mat,i-%d,j-%d,%s);",
	   cell->source[j]->isspecial == TRUE ? special_access : matrix_access,cell->source[j]->offi,
	   cell->source[j]->offj,cell->source[j]->state_source); 
      closebrace(dfp);
      
      
      expr(dfp,"return %s(mat,i - %d,j - %d,%s); ",matrix_access,cell->source[j]->offi,
	   cell->source[j]->offj,cell->source[j]->state_source);

      if( cell->source[j]->position != SOURCE_POS_ALL) {
	closebrace(dfp);
      }
 
      closebrace(dfp);
    }
    expr(dfp,"warn(\"Major problem (!) - in %s read off, position %%d,%%d state %%d no source found!\",i,j,state);",gm->name);
    expr(dfp,"return (-1);");
    closecase(dfp);
  }
  expr(dfp,"default:");
  startcase(dfp);
  expr(dfp,"warn(\"Major problem (!) - in %s read off, position %%d,%%d state %%d no source found!\",i,j,state);",gm->name);
  expr(dfp,"return (-1);");
  closecase(dfp);
  
  
  closebrace(dfp);
}



# line 1340 "dynafunc.dy"
void write_max_calc_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;

  fi = FuncInfo_named_from_varstr(FI_INTERNAL,"max_calc_%s",gm->name);

  start_function_FuncInfo(fi,dfp,"int max_calc_%s(%s * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore,%s_access_func_holder h)",gm->name,gm->name,gm->name);
  
  expr(dfp,"register int temp;");
  expr(dfp,"register int cscore;");
  
  add_break(dfp);
  
  expr(dfp,"*reti = (*retj) = (*retstate) = %s_READ_OFF_ERROR;",gm->name);
  
  add_break(dfp);	
  
  expr(dfp,"if( i < 0 || j < 0 || i > mat->%s->%s || j > mat->%s->%s)",gm->query->name,
       gm->query_len,gm->target->name,gm->target_len);
  startbrace(dfp);
  expr(dfp,"warn(\"In %s matrix special read off - out of bounds on matrix [i,j is %%d,%%d state %%d in standard matrix]\",i,j,state);",gm->name);
  expr(dfp,"return -1");
  closebrace(dfp);

  write_max_calc_block(dfp,gm,"EXPL_MATRIX","EXPL_SPECIAL",TRUE,TRUE);
  
  close_function(dfp);
  
  add_break(dfp);
  
  return;
  

}

static char source_buffer[125];

# line 1377 "dynafunc.dy"
char * source_allowed_statement(int position,int offi,int offj)
{
  switch(position) {
  case SOURCE_POS_ALL :
    warn("Bad point in source to - you have an ALL source positions. Hmmmmm");
    return "1";
  case SOURCE_POS_TOPLEFT :
    sprintf(source_buffer,"(i-%d) == 0 && (j-%d) == 0",offi,offj);
    return source_buffer;
  case SOURCE_POS_TOP :
    sprintf(source_buffer,"(i-%d) == 0 ",offi);
    return source_buffer;
  case SOURCE_POS_LEFT :
    sprintf(source_buffer,"(j-%d) == 0 ",offj);
    return source_buffer;
  case SOURCE_POS_RIGHT :
    return "j == mat->lenj-1";
  case SOURCE_POS_BOTTOM :
    return "i == mat->leni-1";
  case SOURCE_POS_BOTTOMRIGHT :
    return "i == mat->leni-1 && j == mat->lenj-1";
  default :
    warn("Bad error, got an impossible source position %d",position);
    return("1");
  }
}



# line 1406 "dynafunc.dy"
void write_score_block_debug(DYNFILE * dfp,GenericMatrix * gm,char * matrixtag,char * pointertag,char * specialtag,boolean use_special,int debug)
{
  register int i;
  register int j;
  register int k;
  


  for(i=0;i<gm->len;i++) {
    auto CellState * state;
    state = gm->state[i];
    
    add_break(dfp);

    if( state->footprint_start > 1 || state->footprint_end < 0)   {
      add_block_comment(dfp,"State %s has a footprint of %d - %d",state->name,state->footprint_start,state->footprint_end);
      expr(dfp,"if( SEQENDWITHIN(%d) != TRUE || SEQSTARTWITHIN(%d) != TRUE )",state->footprint_end,state->footprint_start);
      startbrace_tag(dfp,"Footprint exists");
    }



    add_block_comment(dfp,"For state %s",state->name);
    add_block_comment(dfp,"setting first movement to score",state->name);
    


    if( state->source[0]->position != SOURCE_POS_ALL) {
      add_block_comment(dfp,"Has restricted position");
      expr(dfp,"if( %s )",source_allowed_statement(state->source[0]->position,state->source[0]->offi,state->source[0]->offj));
      startbrace(dfp);
    }

    /*********************************************************************/
    /* this line looks like                                              */
    /*   score = ProteinMatrix_EXPL_MATRIX(mat,i-1,j-1,MATCH) + xxxxx    */
    /*********************************************************************/
    
    expr(dfp,"score = %s_%s(%s,i-%d,j-%d,%s) + %s",gm->name,matrixtag,pointertag,
	 state->source[0]->offi,state->source[0]->offj,state->source[0]->state_source,
	 state->source[0]->calc_expr);


    if( state->source[0]->isspecial == TRUE ) {
      fatal("Cannot have a special to matrix transition as the first transition");
    }

    if( state->source[0]->position != SOURCE_POS_ALL) {
      closebrace(dfp);
    }

    
    if( dfp->code_debug_level > 5) {
      expr(dfp,"if( score > IMPOSSIBLY_HIGH_SCORE )");
      hang_expr(dfp,"log_full_error(WARNING,5,\"[%%4d][%%4d] State %s source %s Impossibly high score [%%d]\",i,j,score);",state->name,state->source[0]->state_source);
    }

    if( dfp->code_debug_level > 100 ) {
      expr(dfp,"fprintf(stderr,\"MATRIX: [%%4d][%%4d] State %s source %s got score %%d\\n\",i,j,score);",state->name,state->source[0]->state_source);
    }

    /**** ok this is to stop underflow, but is v.v.v. hacky ****/
    /*** removing underflow hack 
    expr(dfp,"if(score < (-10000000) )");
    hang_expr(dfp,"score = (-10000000)");
    ****/
    
    if( debug == 2)
      expr(dfp,"fprintf(stderr,\"[%%4d,%%4d] First score of %s gets %%d\\n\",i,j,score)",state->name);
    
    /****************************************/
    /* now we do if then on score and temp  */
    /****************************************/
    
    for(j=1;j<state->len;j++)	{

      if( use_special == FALSE && state->source[j]->isspecial == TRUE ) 
	continue; /** don't use the special! **/

      if( state->source[j]->position != SOURCE_POS_ALL) {
	add_block_comment(dfp,"Has restricted position");
	expr(dfp,"if( %s )",source_allowed_statement(state->source[j]->position,state->source[j]->offi,state->source[j]->offj));
	startbrace(dfp);
      }



      add_block_comment(dfp,"From state %s to state %s",state->source[j]->state_source,
			state->name);	
      if( state->source[j]->isspecial == TRUE )
	expr(dfp,"temp = %s_%s(%s,i-%d,j-%d,%s) + %s",gm->name,specialtag,pointertag,
	     state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source,
	     state->source[j]->calc_expr);
      else	expr(dfp,"temp = %s_%s(%s,i-%d,j-%d,%s) + %s",gm->name,matrixtag,pointertag,
		     state->source[j]->offi,state->source[j]->offj,state->source[j]->state_source,
		     state->source[j]->calc_expr);
     


      if( dfp->code_debug_level > 5) {
	expr(dfp,"if( temp > IMPOSSIBLY_HIGH_SCORE )");
      hang_expr(dfp,"log_full_error(WARNING,5,\"[%%4d][%%4d] State %s source %s Impossibly high score [%%d]\",i,j,temp);",state->name,state->source[j]->state_source);
      }

      if( dfp->code_debug_level > 100 ) {
	expr(dfp,"fprintf(stderr,\"MATRIX: [%%4d][%%4d] State %s source %s got score %%d\\n\",i,j,temp);",state->name,state->source[0]->state_source);
      }

 
      /**** ok this is to stop underflow, but is v.v.v. hacky ****/
      /**** removing underflow hack
      expr(dfp,"if(score < (-10000000) )");
      hang_expr(dfp,"score = (-10000000)");
      ****/

      if( debug == 2 )
	expr(dfp,"fprintf(stderr,\"[%%4d,%%4d] Score of %s from %s gets %%d\\n\",i,j,temp)",state->name,state->source[j]->state_source );
			
      /** if we have a specified calcfunc - use it here **/
      if(gm->calcfunc != NULL ) {
	expr(dfp,"score = %s(score,temp);",gm->calcfunc);
      } else{

	expr(dfp,"if( temp  > score )");
	startbrace(dfp);
	expr(dfp,"score = temp;");
	/** ok for shadow matrix should put things in here */
	closebrace(dfp);

      }
      if( state->source[j]->position != SOURCE_POS_ALL) {
	closebrace(dfp);
      }

    }
    
    /************************/
    /* finished blocks      */
    /* put in global calc   */
    /************************/
    add_break(dfp);
    add_block_comment(dfp,"Ok - finished max calculation for %s",state->name);
    add_block_comment(dfp,"Add any movement independant score and put away"); 
    
    if( state->calc_expr != NULL)
      expr(dfp," score += %s",state->calc_expr); 

    /***************************/			
    /* put away score          */
    /***************************/
    
    expr(dfp," %s_%s(%s,i,j,%s) = score;",gm->name,matrixtag,pointertag,state->name);


    if( use_special == FALSE ) {
      add_block_comment(dfp,"Finished calculating state %s",state->name);
      continue;
    }

    
    
    /************************/
    /* for each special     */
    /* thats has this as    */
    /* source we have to    */
    /* update               */
    /************************/
    
    for(j=0;j<gm->spec_len;j++) {
      auto CellState * specstate;
      specstate = gm->special[j];
      
      
      for(k=0;k<specstate->len;k++) {
	if( strcmp(specstate->source[k]->state_source,state->name) == 0) {
	  /********************************/
	  /* is a special source!         */
	  /********************************/
	  add_break(dfp);
	  add_block_comment(dfp,"state %s is a source for special %s",state->name,specstate->name);


	  if( specstate->source[k]->position != SOURCE_POS_ALL) {
	    add_block_comment(dfp,"Has restricted position");
	    expr(dfp,"if( %s )",source_allowed_statement(specstate->source[k]->position,specstate->source[k]->offi,specstate->source[k]->offj));
	    startbrace(dfp);
	  }


	  expr(dfp,"temp = score + (%s) + (%s) ",specstate->source[k]->calc_expr,specstate->calc_expr == NULL ? "0" : specstate->calc_expr );


	  if(gm->calcfunc != NULL ) {
	    expr(dfp,"%s_%s(%s,i,j,%s) = %s(%s_%s(%s,i,j,%s),temp);",
		 gm->name,specialtag,pointertag,specstate->name,
		 gm->calcfunc,
		 gm->name,specialtag,pointertag,specstate->name);
	    
	  } else{

	    expr(dfp,"if( temp > %s_%s(%s,i,j,%s) ) ",gm->name,specialtag,pointertag,specstate->name);
	    startbrace(dfp);
	    expr(dfp,"%s_%s(%s,i,j,%s) = temp",gm->name,specialtag,pointertag,specstate->name);
	    if( debug >= 1 )
	      expr(dfp,"fprintf(stderr,\"[%%4d,%%4d] new score of %s is %%d\\n\",i,j,temp)",state->name);
	    
	    closebrace(dfp);
	    add_break(dfp);

	  }
	  if( specstate->source[k]->position != SOURCE_POS_ALL) {
	    closebrace(dfp);
	  }
	  
	} /* end of if this special state was a source for previous guy */
	

      } /* end for each source of the special state */

      if( dfp->code_debug_level > 4) {
	expr(dfp,"if( %s_%s(mat,0,j,%s) > IMPOSSIBLY_HIGH_SCORE )",gm->name,specialtag,specstate->name);
	hang_expr(dfp,"log_full_error(WARNING,5,\"[%%4d][%%4d] Special state %s Impossibly high score [%%d] found\",i,j,%s_%s(mat,0,j,%s));",specstate->name,gm->name,specialtag,specstate->name);
      }


    }  /* end for each special state */
    

    
    if( state->footprint_start < 0 || state->footprint_end > 1 )
      {
	closebrace(dfp);
      }
    
    
    add_break(dfp);
    add_block_comment(dfp,"Finished calculating state %s",state->name);
    
  } /* end of for each state */
	
}


# line 1649 "dynafunc.dy"
void write_score_block(DYNFILE * dfp,GenericMatrix * gm,char * matrixtag,char * pointertag,char * specialtag,boolean use_special)
{
  write_score_block_debug(dfp,gm,matrixtag,pointertag,specialtag,use_special,0);
}


/* Function:  matrix_calculate_func(dfp,gm)
 *
 * Descrip:    makes calculate_xxx functions, which
 *             is for explicit matrix implementations
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 1659 "dynafunc.dy"
void matrix_calculate_func(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  ArgInfo * ai;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"calculate_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the %s matrix when in explicit mode",gm->name);
  add_line_to_Ftext(fi->ft,"To allocate the matrix use /allocate_Expl_%s",gm->name);


  /** ArgInfo_in_FuncInfo_from_varstr automatically adds ai to fi **/
  
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"mat");
  ai->desc=stringallocf("%s which contains explicit basematrix memory",gm->name);


  start_function_FuncInfo(fi,dfp,"boolean calculate_%s(%s * mat)",gm->name,gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int leni;");
  expr(dfp,"int lenj;");
  expr(dfp,"long int tot;");
  expr(dfp,"long int num;");
  add_break(dfp);
  
  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT )");
  startbrace(dfp);
  warn_expr(dfp,"in calculate_%s, passed a non Explicit matrix type, cannot calculate!",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  add_break(dfp);
  expr(dfp,"leni = mat->leni;");
  expr(dfp,"lenj = mat->lenj;");

  
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
  

/**  expr(dfp,"(void) do_cell_%s(mat,i,j)",gm->name); **/

  expr(dfp,"num++;"); 

  write_score_block(dfp,gm,"EXPL_MATRIX","mat","EXPL_SPECIAL",TRUE);


  closebrace(dfp);
  /**** if there are any specials do them here ****/

  write_special_block(dfp,gm,"EXPL_MATRIX","EXPL_SPECIAL",NULL);
  
  closebrace(dfp);
  
  /*** stop reporting ***/
  
  expr(dfp,"stop_reporting()");
    
  expr(dfp,"return TRUE");
  
  close_function(dfp);
  
  add_break(dfp);
  
}	



/* Function:  matrix_calculate_func_dpenv(dfp,gm)
 *
 * Descrip:    makes calculate_xxx functions, which
 *             is for explicit matrix implementations
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 1748 "dynafunc.dy"
void matrix_calculate_func_dpenv(DYNFILE * dfp,GenericMatrix * gm)
{
  FuncInfo * fi;
  ArgInfo * ai;
  int i;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"calculate_dpenv_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function calculates the %s matrix when in explicit mode, subject to the envelope",gm->name);


  /** ArgInfo_in_FuncInfo_from_varstr automatically adds ai to fi **/
  
  ai =  ArgInfo_in_FuncInfo_from_varstr(fi,"mat");
  ai->desc=stringallocf("%s which contains explicit basematrix memory",gm->name);


  start_function_FuncInfo(fi,dfp,"boolean calculate_dpenv_%s(%s * mat,DPEnvelope * dpenv)",gm->name,gm->name);
  expr(dfp,"int i;");
  expr(dfp,"int j;");
  expr(dfp,"int k;");
  expr(dfp,"int starti;");
  expr(dfp,"int startj;");
  expr(dfp,"int endi;");
  expr(dfp,"int endj;");
  expr(dfp,"int tot;");
  expr(dfp,"int num;");
  expr(dfp,"int should_calc;");
  add_break(dfp);
  
  expr(dfp,"if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT )");
  startbrace(dfp);
  warn_expr(dfp,"in calculate_%s, passed a non Explicit matrix type, cannot calculate!",gm->name);
  expr(dfp,"return FALSE;");
  closebrace(dfp);
  add_break(dfp);

  
  /*** make num and tot ***/
  
  expr(dfp,"prepare_DPEnvelope(dpenv)");

  expr(dfp,"starti = dpenv->starti");
  expr(dfp,"if( starti < 0 )");
  hang_expr(dfp,"starti = 0");

  expr(dfp,"startj = dpenv->startj");
  expr(dfp,"if( startj < 0 )");
  hang_expr(dfp,"startj = 0");

  expr(dfp,"endi = dpenv->endi");
  expr(dfp,"if( endi > mat->leni )");
  hang_expr(dfp,"endi = mat->leni");
  
  expr(dfp,"endj = dpenv->endj");
  expr(dfp,"if( endj > mat->lenj )");
  hang_expr(dfp,"endj = mat->lenj");
  

  expr(dfp,"tot = (endi-starti) * (endj-startj);");
  expr(dfp,"num = 0;");

  add_break(dfp);
  expr(dfp,"for(j=startj-%d;j<endj;j++)",gm->window_j);
  startbrace(dfp);
  expr(dfp,"for(i=%d;i<mat->leni;i++)",gm->window_i);
  startbrace(dfp);
  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_EXPL_MATRIX(mat,i,j,%s) = %s;",gm->name,gm->state[i]->name,gm->state[i]->def_score);
  }
  closebrace(dfp);
  closebrace(dfp);


  expr(dfp,"for(j=-%d;j<mat->lenj;j++)",gm->window_j);
  startbrace(dfp);
  for(i=0;i<gm->spec_len;i++) {
    expr(dfp,"%s_EXPL_SPECIAL(mat,i,j,%s) = %s;",gm->name,gm->special[i]->name,gm->special[i]->def_score);
  }
  closebrace(dfp);


  /*** see if there any specials to specials to do ***/
  
  add_break(dfp);
  expr(dfp,"start_reporting(\"%s Matrix calculation: \");",gm->name);
  
  expr(dfp,"for(j=startj;j<endj;j++)");
  startbrace(dfp);
  expr(dfp,"auto int score");
  expr(dfp,"auto int temp");
  expr(dfp,"for(i=starti;i<endi;i++)");
  startbrace(dfp);

  add_block_comment(dfp,"Check if is in envelope - code identical to is_in_DPEnvelope, but aggressively inlined here for speed");

  expr(dfp,"should_calc = 0;");
  expr(dfp,"for(k=0;k<dpenv->len;k++)");
  startbrace(dfp);
  expr(dfp,"auto DPUnit * u;");
  expr(dfp,"u = dpenv->dpu[k];");
  expr(dfp,"switch(u->type)");
  startbrace(dfp);
  expr(dfp,"case DPENV_RECT :");
  startcase(dfp);
  expr(dfp,"if( i >= u->starti && j >= u->startj && i <= (u->starti+u->height) && j <= (u->startj+u->length))");
  hang_expr(dfp,"should_calc = 1");
  expr(dfp,"break;");
  closecase(dfp);
  expr(dfp,"case DPENV_DIAG :");
  startcase(dfp);
  expr(dfp,"if(  abs( (i-j) - (u->starti-u->startj)) <= u->height && i+j >= u->starti+u->startj && i+j+u->length >= u->starti+u->startj)");
  hang_expr(dfp,"should_calc = 1");
  expr(dfp,"break;");
  closecase(dfp);
  closebrace(dfp);
  expr(dfp,"if( should_calc == 1 )");
  hang_expr(dfp,"break;");
  closebrace(dfp);

  expr(dfp,"if( should_calc == 0)");
  startbrace(dfp);
  for(i=0;i<gm->len;i++) {
    expr(dfp,"%s_EXPL_MATRIX(mat,i,j,%s) = %s;",gm->name,gm->state[i]->name,gm->state[i]->def_score);
  }
  expr(dfp,"continue;");
  closebrace(dfp);


  add_break(dfp);


  expr(dfp,"if( num%%1000 == 0 )");
  hang_expr(dfp,"log_full_error(REPORT,0,\"[%%7d] Cells %%2d%%%%%%%%\",num,num*100/tot);");
  

/**  expr(dfp,"(void) do_cell_%s(mat,i,j)",gm->name); **/

  expr(dfp,"num++;"); 

  write_score_block(dfp,gm,"EXPL_MATRIX","mat","EXPL_SPECIAL",TRUE);


  closebrace(dfp);
  /**** if there are any specials do them here ****/

  write_special_block(dfp,gm,"EXPL_MATRIX","EXPL_SPECIAL",NULL);
  
  closebrace(dfp);
  
  /*** stop reporting ***/
  
  expr(dfp,"stop_reporting()");
    
  expr(dfp,"return TRUE");
  
  close_function(dfp);
  
  add_break(dfp);
  
}	



# line 1911 "dynafunc.dy"
void do_special_function(DYNFILE * dfp,GenericMatrix * gm)
{

  if( gm->specialtospecial == FALSE ) {
    warn("Cannot make special cell function if no special to special use");
    return;
  }

  start_function(dfp,"void do_special_cells_%s(%s * mat,int j)",gm->name,gm->name);

  expr(dfp,"register int score");
  expr(dfp,"register int temp");

  write_special_block(dfp,gm,"EXPL_MATRIX","EXPL_SPECIAL",NULL);

  add_break(dfp);

  add_break(dfp);
  expr(dfp,"return;");
  close_function(dfp);
  add_break(dfp);

}

/* Function:  do_cell_function(dfp,gm)
 *
 * Descrip:    useful function for debugging, but not
 *             currently used
 *
 *
 * Arg:        dfp [UNKN ] Undocumented argument [DYNFILE *]
 * Arg:         gm [UNKN ] Undocumented argument [GenericMatrix *]
 *
 */
# line 1939 "dynafunc.dy"
void do_cell_function(DYNFILE * dfp,GenericMatrix * gm)
{
  start_function(dfp,"int do_cell_%s(%s * mat,int i,int j)",gm->name,gm->name);
  
  expr(dfp,"register int score;");
  expr(dfp,"register int temp;");
  expr(dfp,"register int state;");
  
  write_score_block(dfp,gm,"EXPL_MATRIX","mat","EXPL_SPECIAL",TRUE);
  
  expr(dfp,"return 0");
  
  close_function(dfp);


  add_break(dfp);
}

/* Function:  write_special_block(dfp,gm,matrix,special,bestscore)
 *
 * Descrip:    This writes the special to special block movements, if needed.
 *
 *             If bestscore is not NULL, it assummes that this is the name of
 *             an int variable which should hold the bestscore by updating 
 *             from the end state
 *
 *
 * Arg:              dfp [UNKN ] dynfile output [DYNFILE *]
 * Arg:               gm [READ ] matrix structure [GenericMatrix *]
 * Arg:           matrix [READ ] name of main matrix tag eg EXPL_MATRIX [char *]
 * Arg:          special [READ ] name of special tag  eg   EXPL_SPECIAL [char *]
 * Arg:        bestscore [READ ] if not NULL, name of scoped variable for updating bestscore [char *]
 *
 */
# line 1971 "dynafunc.dy"
void write_special_block(DYNFILE * dfp,GenericMatrix * gm,char * matrix,char * special,char * bestscore)
{
  register int i;
  register int j;

  for(i=0;i<gm->spec_len;i++) {
    auto CellState * state;
    state = gm->special[i];
    add_break(dfp);
    if( state->specialtospecial == FALSE ) {
      add_block_comment(dfp,"Special state %s has no special to special movements",state->name);
      continue;
    }

    add_block_comment(dfp,"Special state %s has special to speical",state->name);
    
    add_block_comment(dfp,"Set score to current score (remember, state probably updated during main loop");
    expr(dfp,"score = %s_%s(mat,0,j,%s);",gm->name,special,state->name);
    add_break(dfp);
    for(j=0;j < state->len;j++) {
      auto CellSource * source;
      source = state->source[j];



      if( source->isspecial == FALSE ) {
	add_block_comment(dfp,"Source %s for state %s is not special... already calculated",source->state_source,state->name);
	continue; /** back to for j **/
      }

      add_block_comment(dfp,"Source %s is a special source for %s",source->state_source,state->name);

      if( state->source[j]->position != SOURCE_POS_ALL) {
	add_block_comment(dfp,"Has restricted position");
	expr(dfp,"if( %s )",source_allowed_statement(state->source[j]->position,state->source[j]->offi,state->source[j]->offj));
	startbrace(dfp);
      }


      expr(dfp,"temp = %s_%s(mat,0,j - %d,%s) + (%s) + (%s)",gm->name,special,source->offj,source->state_source,
source->calc_expr,state->calc_expr == NULL ? "0" : state->calc_expr);

      if( gm->calcfunc != NULL ) {
	expr(dfp,"score = %s(score,temp);",gm->calcfunc);
      } else {
	expr(dfp,"if( temp > score )");
	hang_expr(dfp,"score = temp");
      }

      if( state->source[j]->position != SOURCE_POS_ALL) {
	closebrace(dfp);
      }

      add_break(dfp);
    }
    add_block_comment(dfp,"Put back score... (now updated!)");
    expr(dfp,"%s_%s(mat,0,j,%s) = score;",gm->name,special,state->name);
    add_block_comment(dfp,"Finished updating state %s",state->name);
    add_break(dfp);

    if( dfp->code_debug_level > 5) {
      expr(dfp,"if( score > IMPOSSIBLY_HIGH_SCORE )");
      hang_expr(dfp,"log_full_error(WARNING,5,\"[%%4d][%%4d] Special state %s Impossibly high score [%%d] found\",i,j,score);",state->name);
    }
  }
  /*
  if( bestscore == NULL && gm->calcfunc == NULL )
    return;
  */

  /** ok - update of endscore stuff **/

  for(i=0;i<gm->spec_len;i++) {
    auto CellState * state;
    state = gm->special[i];

    if( state->is_end == TRUE ) {

      if( bestscore == NULL ) {
	/* means we have a calc func with no bestscore */
	/*expr(dfp,"%s_%s(mat,0,j,%s) = %s(%s_%s(mat,0,j,%s),%s_%s(mat,0,j-1,%s));",gm->name,special,state->name,gm->calcfunc,gm->name,special,state->name,gm->name,special,state->name);*/
      } else {
	/* we are in a database search routine */
	if( gm->calcfunc != NULL ) {
	  expr(dfp,"bestscore = %s(bestscore,%s_%s(mat,0,j,%s));",gm->calcfunc,gm->name,special,state->name);
	} else {
	  expr(dfp,"if( bestscore < %s_%s(mat,0,j,%s) )",gm->name,special,state->name);
	  hang_expr(dfp,"bestscore = %s_%s(mat,0,j,%s)",gm->name,special,state->name);
	}
      }

      break;
    }
  }

}

# line 2068 "dynafunc.dy"
void write_search_distributor_func(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  int i;
  char buffer[MAXLINE];
  FuncInfo * fi;
  boolean qdb = FALSE;
  boolean tdb = FALSE;

  fi = FuncInfo_named_from_varstr(FI_CALLABLE,"search_%s",gm->name);
  add_line_to_Ftext(fi->ft,"This function makes a database search of %s",gm->name);
  add_line_to_Ftext(fi->ft,"It uses the dbsi structure to choose which implementation");
  add_line_to_Ftext(fi->ft,"to use of the database searching. This way at run time you");
  add_line_to_Ftext(fi->ft,"can switch between single threaded/multi-threaded or hardware");

  if( gm->qtype != NULL && gm->qtype->is_database == TRUE) 
    qdb = TRUE;
  if( gm->ttype != NULL && gm->ttype->is_database == TRUE) 
    tdb = TRUE;

  if( qdb == TRUE)
    sprintf(buffer,"Search_Return_Type search_%s(DBSearchImpl * dbsi,Hscore * out,%s querydb,",gm->name,gm->qtype->database_type);
  else sprintf(buffer,"Search_Return_Type search_%s(DBSearchImpl * dbsi,Hscore * out,%s %s,",gm->name,gm->query->element_type,gm->query->name);
  
  if( tdb == TRUE) {
    strcat(buffer,gm->ttype->database_type);
    strcat(buffer," targetdb ");
  } else {
    strcat(buffer,gm->target->element_type);
    strcat(buffer," ");
    strcat(buffer,gm->target->name);
    strcat(buffer," ");
  }
  
  for(i=0;i<gm->res_len;i++) {
    strcat(buffer,",");
    strcat(buffer,gm->resource[i]->element_type);
    strcat(buffer," ");
    strcat(buffer,gm->resource[i]->name);
  }
  strcat(buffer,")");


  start_function_FuncInfo(fi,dfp,buffer);
  if( dpi->do_threads == TRUE && can_do_threads(gm) == TRUE ) {
    macro(dfp,"#ifdef PTHREAD");
    expr(dfp,"int i;");
    expr(dfp,"int thr_no;");
    expr(dfp,"pthread_attr_t pat;");
    expr(dfp,"struct thread_pool_holder_%s * holder",gm->name);
    macro(dfp,"#endif");
  }

  expr(dfp,"if( out == NULL )");
  startbrace(dfp);
  warn_expr(dfp,"Passed in a null Hscore object into search_%s. Can't process results!",gm->name);
  expr(dfp,"return SEARCH_ERROR;");
  closebrace(dfp);

  expr(dfp,"if( dbsi == NULL )");
  startbrace(dfp);
  warn_expr(dfp,"Passed in a null DBSearchImpl object into search_%s. Can't process results!",gm->name);
  expr(dfp,"return SEARCH_ERROR;");
  closebrace(dfp);

  if( dpi->db_trace_level == 0 ) {
    expr(dfp,"if( dbsi->trace_level > 0 ) ");
    hang_expr(dfp,"warn(\"Although you are asking at run-time for database tracing, the %s matrix was not compiled with database tracing. No tracing will be made\");",gm->name);
  } else {
    expr(dfp,"if( dbsi->trace_level > %d ) ",dpi->db_trace_level);
    hang_expr(dfp,"warn(\"Asking for trace level of %%d in database search for %s, but it was compiled with a trace level of %d. Not all trace statements can be shown\",dbsi->trace_level);",gm->name);
  }

  
  /* now to figure out what implementations should be run! */
  
  expr(dfp,"switch(dbsi->type)");
  startbrace_tag(dfp,"switch on implementation");
  expr(dfp,"case DBSearchImpl_Serial :");
  startcase(dfp);
  if( qdb == TRUE)
    sprintf(buffer,"return serial_search_%s(out,querydb,",gm->name);
  else sprintf(buffer,"return serial_search_%s(out,%s,",gm->name,gm->query->name);

  if( tdb == TRUE) {
    strcat(buffer," targetdb ");
  } else {
    strcat(buffer,gm->target->name);
    strcat(buffer," ");
  }
  
  for(i=0;i<gm->res_len;i++) {
    strcat(buffer,",");
    strcat(buffer,gm->resource[i]->name);
  }
  strcat(buffer,");");

  expr(dfp,buffer);
  closecase(dfp);

  expr(dfp,"case DBSearchImpl_Pthreads :");
  startcase(dfp);

  if( dpi->do_threads == TRUE && can_do_threads(gm) == TRUE ) {
    macro(dfp,"#ifdef PTHREAD");
    expr(dfp,"holder = (struct thread_pool_holder_%s *) ckalloc(sizeof(struct thread_pool_holder_%s));",gm->name,gm->name);
    expr(dfp,"if( holder == NULL ) ");
    startbrace(dfp);
    warn_expr(dfp,"Unable to allocated thread pool datastructure...");
    expr(dfp,"return SEARCH_ERROR;");
    closebrace(dfp);
    expr(dfp,"holder->out = out;");
    if( dpi->db_trace_level > 0 ) 
      expr(dfp,"holder->dbsi = dbsi;");
    
    if( qdb == TRUE ) {
      expr(dfp,"holder->querydb = querydb;");
    } else {
      expr(dfp,"holder->%s = %s;",gm->query->name,gm->query->name);
    }
    
    if( tdb == TRUE ) {
      expr(dfp,"holder->targetdb = targetdb;");
    } else {
      expr(dfp,"holder->%s = %s;",gm->target->name,gm->target->name);
    }
    
    for(i=0;i<gm->res_len;i++) {
      expr(dfp,"holder->%s = %s;",gm->resource[i]->name,gm->resource[i]->name);
    }
    expr(dfp,"if( pthread_mutex_init(&(holder->input_lock),NULL) != 0 ) ");
    expr(dfp,"fatal(\"Unable to iniated input mutex lock\");");
    expr(dfp,"if( pthread_mutex_init(&(holder->output_lock),NULL) != 0 ) ");
    expr(dfp,"fatal(\"Unable to iniated output mutex lock\");");

    
    add_block_comment(dfp,"Let us rock!");
    expr(dfp,"thr_no = number_of_threads_DBSearchImpl(dbsi);");
    expr(dfp,"holder->pool = ckcalloc (thr_no,sizeof(pthread_t));");
    expr(dfp,"if( holder->pool == NULL ) ");
    startbrace(dfp);
    warn_expr(dfp,"Unable to allocated thread pools");
    expr(dfp,"return SEARCH_ERROR;");
    closebrace(dfp);

    add_block_comment(dfp,"Build a thread attribute to make sure we get the most out of SMP boxes");
    expr(dfp,"pthread_attr_init(&pat)");
    add_block_comment(dfp,"Give thread libraries a hint that threads should be kernel threads");
    macro(dfp,"#ifndef __sgi /* SGI can't set system scope ... */");
    macro(dfp,"#ifdef  HAS_PTHREAD_SETSCOPE");
    expr(dfp,"pthread_attr_setscope(&pat, PTHREAD_SCOPE_SYSTEM);");
    macro(dfp,"#endif /* set scope */");
    macro(dfp,"#endif /* sgi */");
    add_block_comment(dfp,"Give thread libraries a hint that there are num of threads to run");
    macro(dfp,"#ifdef HAS_PTHREAD_SETCONCURRENCY");
    expr(dfp,"pthread_setconcurrency(thr_no+1);");
    macro(dfp,"#endif /* set concurrency */");

    expr(dfp,"for(i=0;i<thr_no;i++)");
    startbrace(dfp);
    expr(dfp,"if( pthread_create(holder->pool+i,&pat,thread_loop_%s,(void *)holder) )",gm->name);
    hang_expr(dfp,"fatal(\"Unable to create a thread!\");");
    
    closebrace(dfp);
    

    add_block_comment(dfp,"Now - wait for all the threads to exit");
    
    expr(dfp,"for(i=0;i<thr_no;i++)");
    startbrace(dfp);
    expr(dfp,"if( pthread_join(holder->pool[i],NULL) != 0 )",gm->name);
    hang_expr(dfp,"fatal(\"Unable to join a thread!\");");
    
    closebrace(dfp);

    /*    expr(dfp,"pthread_attr_destroy(&pat);"); */
    
    add_block_comment(dfp,"Deallocate the thread structures");
    expr(dfp,"ckfree(holder->pool)");
    expr(dfp,"ckfree(holder)");
    
    expr(dfp,"return SEARCH_OK;");
    macro(dfp,"#else /* not compiled with threads */");
    warn_expr(dfp,"You did not specifiy the PTHREAD compile when compiled the C code for %s",gm->name);
    macro(dfp,"#endif /* finished threads */");
  } else {
    warn_expr(dfp,"This matrix %s was not dyc compiled with thread support",gm->name);
    expr(dfp,"return SEARCH_ERROR;");
  }
  
  closecase(dfp);
  expr(dfp,"default :");
  startcase(dfp);
  expr(dfp,"warn(\"database search implementation %%s was not provided in the compiled dynamite file from %s\",impl_string_DBSearchImpl(dbsi));",gm->name);
  expr(dfp,"return SEARCH_ERROR;");
  closecase(dfp);
  closebrace(dfp);

  add_break(dfp);
  close_function(dfp);
  add_break(dfp);
 
}  



# line 2273 "dynafunc.dy"
void write_GenericMatrix_header(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{
  StructHolder * temp;
  StructElement * se;
  char buffer[MAXLINE];

  write_StructHolder_header(dfp,gm->sh);
  if( dpi->do_threads == TRUE ) {
    if( can_do_threads(gm) == FALSE ) {
      warn("Asking for threads, but the types in this matrix %s are not thread safe",gm->name);
    } else {
      write_thread_struct(dfp,dpi,gm);
    }
  }

  if( dpi->dyvector == TRUE ) {
    write_naive_vector_struct(dfp,dpi,gm);
  }
  
  if( dpi->doprob == TRUE ) {
    write_forwardbackward_struct(dfp,dpi,gm);
  }

  
  temp = StructHolder_alloc_std();
  sprintf(buffer,"%s_access_func_holder",gm->name);
  temp->name = stringalloc(buffer);

  sprintf(buffer,"int (*access_main)(%s*,int,int,int)",gm->name);
  se = basic_add_StructElement(temp,"access_main","int");
  se->element_type = stringalloc(buffer);
  se->isfunc = TRUE;

  sprintf(buffer,"int (*access_special)(%s*,int,int,int)",gm->name);
  se = basic_add_StructElement(temp,"access_special","int");
  se->element_type = stringalloc(buffer);
  se->isfunc = TRUE;

  write_StructHolder_header(dfp,temp);

  free_StructHolder(temp);

/**	write_StructHolder_header(gm->search_sh,ofp); **/
  
  /**
     load_file(ofp);

     write_memory_macros(gm);
     
     close_file();
  **/

}

/*****************************************
   this function writes the C functions themselves

   it does not load up ofp into the header part of
   writec, and you must do that first if you want 
   the function prototypes to be put into the header
   files

******************************************/

# line 2337 "dynafunc.dy"
void write_GenericMatrix_func(DYNFILE * dfp,GenericMatrix * gm,DPImplementation * dpi)
{

  fprintf(dfp->func,"\n\n  /*****************   C functions  ****************/\n");
  fprintf(dfp->func,"  /*             Written using dynamite            */\n");
  fprintf(dfp->func,"  /*            %s           */\n",now_string());
  fprintf(dfp->func,"  /*            email birney@sanger.ac.uk          */\n");
  fprintf(dfp->func,"  /* http://www.sanger.ac.uk/Users/birney/dynamite */\n");
  fprintf(dfp->func,"  /*************************************************/\n");   	
  fprintf(dfp->func,"\n\n");
  fprintf(dfp->func,"  /* Please report any problems or bugs to         */\n");
  fprintf(dfp->func,"  /* Ewan Birney, birney@sanger.ac.uk              */\n");
  fprintf(dfp->func,"\n\n");
  
  
  
  add_block_comment(dfp,"basic set of macros to map states to numbers");

  write_memory_macros(dfp,gm);
  write_search_macros(dfp,gm,dpi);
  
  add_break(dfp);

  if( dpi->dydebug == TRUE ) {
    write_debug_funcs(dfp,gm);
  }

  write_shatter_functions(dfp,gm,dpi);


  write_search_distributor_func(dfp,gm,dpi);

  if( dpi->do_threads == TRUE && can_do_threads(gm) ) {
    write_thread_loop(dfp,dpi,gm);
  }

  if( dpi->dyvector == TRUE ) {
    write_naive_vector(dfp,dpi,gm);
  }


  if( dpi->doprob == TRUE ) {
    write_probabilistic_models(dfp,gm,dpi);
  }

  make_search_loop_function(dfp,gm);



  write_one_score_GenericMatrix(dfp,gm,dpi);


/**  write_database_scan_function(gm); **/
/**
  if( make_Compugen_functions_if_possible(gm) == TRUE) {
    log_full_error(INFO,0,"Able to make Compugen functions");
  }
**/

  /*** testing ***/

  one_shot_aln_func(dfp,gm,dpi);

  write_safe_alloc_function(dfp,gm);

  alloc_expl_func_GenericMatrix(dfp,gm);

  init_matrix_func(dfp,gm);

  recalculate_PackAln_func(dfp,gm);

  /*** this is found in dynshadow.dy ***/

  add_block_comment(dfp,"divide and conquor macros are next");
  write_dc_functions(dfp,gm);

  /*write_pal_to_ars_func(dfp,gm);*/

  write_aln_conversion_func(dfp,gm);

  write_alncconvert_make_func(dfp,gm);
  
  write_expl_read_func(dfp,gm);

  write_expl_access_funcs(dfp,gm);


  write_basic_read_func(dfp,gm);

  

  find_end_func(dfp,gm);
  
  debug_func(dfp,gm);
  
  write_max_calc_func(dfp,gm);
  
  write_special_max_calc_func(dfp,gm);

  matrix_calculate_func(dfp,gm);

  matrix_calculate_func_dpenv(dfp,gm);
  
  write_simplealloc_function(dfp,gm->sh);
  write_free_function(dfp,gm->sh);
  
   
  add_break(dfp);
  
  return;
}	

# line 2494 "dynafunc.c"

#ifdef _cplusplus
}
#endif
