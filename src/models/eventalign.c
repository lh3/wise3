#ifdef _cplusplus
extern "C" {
#endif
#include "eventalign.h"





# line 9 "eventalign.c"


  /*****************   C functions  ****************/
  /*             Written using dynamite            */
  /*            Thu Apr 17 09:17:28 2014           */
  /*            email birney@sanger.ac.uk          */
  /* http://www.sanger.ac.uk/Users/birney/dynamite */
  /*************************************************/


  /* Please report any problems or bugs to         */
  /* Ewan Birney, birney@sanger.ac.uk              */


/* basic set of macros to map states to numbers */ 
#define MATCH 0  
#define INSERT_SIGNAL 1  
#define DELETE 2 
#define DIFF_SEQ 3   


#define START 0  
#define END 1    


#define EventSignalMat_EXPL_MATRIX(this_matrix,i,j,STATE) this_matrix->basematrix->matrix[((j+6)*4)+STATE][i+6]  
#define EventSignalMat_EXPL_SPECIAL(matrix,i,j,STATE) matrix->basematrix->specmatrix[STATE][j+6] 
#define EventSignalMat_READ_OFF_ERROR -13
   


#define EventSignalMat_VSMALL_MATRIX(mat,i,j,STATE) mat->basematrix->matrix[(j+7)%7][((i+6)*4)+STATE]    
#define EventSignalMat_VSMALL_SPECIAL(mat,i,j,STATE) mat->basematrix->specmatrix[(j+7)%7][STATE] 




#define EventSignalMat_SHATTER_SPECIAL(matrix,i,j,STATE) matrix->shatter->special[STATE][j]  
#define EventSignalMat_SHATTER_MATRIX(matrix,i,j,STATE)  fetch_cell_value_ShatterMatrix(mat->shatter,i,j,STATE)  


/* Function:  PackAln_read_Shatter_EventSignalMat(mat)
 *
 * Descrip:    Reads off PackAln from shatter matrix structure
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_read_Shatter_EventSignalMat(EventSignalMat * mat) 
{
    EventSignalMat_access_func_holder holder;    


    holder.access_main    = EventSignalMat_shatter_access_main;  
    holder.access_special = EventSignalMat_shatter_access_special;   
    assert(mat);     
    assert(mat->shatter);    
    return PackAln_read_generic_EventSignalMat(mat,holder);  
}    


/* Function:  EventSignalMat_shatter_access_main(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int EventSignalMat_shatter_access_main(EventSignalMat * mat,int i,int j,int state) 
{
    return EventSignalMat_SHATTER_MATRIX(mat,i,j,state); 
}    


/* Function:  EventSignalMat_shatter_access_special(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int EventSignalMat_shatter_access_special(EventSignalMat * mat,int i,int j,int state) 
{
    return EventSignalMat_SHATTER_SPECIAL(mat,i,j,state);    
}    


/* Function:  calculate_shatter_EventSignalMat(mat,dpenv)
 *
 * Descrip:    This function calculates the EventSignalMat matrix when in shatter mode
 *
 *
 * Arg:          mat [UNKN ] (null) [EventSignalMat *]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean calculate_shatter_EventSignalMat(EventSignalMat * mat,DPEnvelope * dpenv) 
{
    int i;   
    int j;   
    int k;   
    int should_calc;     
    int leni;    
    int lenj;    
    int tot; 
    int num; 
    int starti;  
    int startj;  
    int endi;    
    int endj;    


    int * SIG_0_0;   
    int * SIG_1_1;   
    int * SIG_0_1;   
    int * SIG_1_0;   
    int * SIG_5_5;   
    int * SIG_6_6;   


    leni = mat->leni;    
    lenj = mat->lenj;    


    mat->shatter = new_ShatterMatrix(dpenv,4,lenj,2);    
    prepare_DPEnvelope(dpenv);   
    starti = dpenv->starti;  
    if( starti < 0 ) 
      starti = 0;    
    startj = dpenv->startj;  
    if( startj < 0 ) 
      startj = 0;    
    endi = dpenv->endi;  
    if( endi > mat->leni )   
      endi = mat->leni;  
    endj = dpenv->endj;  
    if( endj > mat->lenj )   
      endj = mat->lenj;  
    tot = (endi-starti) * (endj-startj); 
    num = 0; 


    start_reporting("EventSignalMat Matrix calculation: ");  
    for(j=startj;j<endj;j++) {  
      auto int score;    
      auto int temp;     
      for(i=starti;i<endi;i++)   {  
        /* Check if is in envelope - code identical to is_in_DPEnvelope, but aggressively inlined here for speed */ 
        should_calc = 0; 
        for(k=0;k<dpenv->len;k++)    {  
          auto DPUnit * u;   
          u = dpenv->dpu[k]; 
          switch(u->type)    {  
            case DPENV_RECT :    
              if( i >= u->starti && j >= u->startj && i < (u->starti+u->height) && j < (u->startj+u->length))    
                should_calc = 1;     
              break; 
            case DPENV_DIAG :    
              if(  abs( (i-j) - (u->starti-u->startj)) <= u->height && i+j >= u->starti+u->startj && i+j+u->length >= u->starti+u->startj)   
                should_calc = 1;     
              break; 
            }  
          if( should_calc == 1 ) 
            break;   
          }  
        if( should_calc == 0)    
          continue;  


        SIG_0_0 = fetch_cell_from_ShatterMatrix(mat->shatter,i,j);   
        SIG_1_1 = fetch_cell_from_ShatterMatrix(mat->shatter,i-1,j-1);   
        SIG_0_1 = fetch_cell_from_ShatterMatrix(mat->shatter,i-0,j-1);   
        SIG_1_0 = fetch_cell_from_ShatterMatrix(mat->shatter,i-1,j-0);   
        SIG_5_5 = fetch_cell_from_ShatterMatrix(mat->shatter,i-5,j-5);   
        SIG_6_6 = fetch_cell_from_ShatterMatrix(mat->shatter,i-6,j-6);   




        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = SIG_1_1[DIFF_SEQ] + 0;   
        /* From state MATCH to state MATCH */ 
        temp = SIG_1_1[MATCH] + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = SIG_1_1[INSERT_SIGNAL] + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DELETE to state MATCH */ 
        temp = SIG_1_1[DELETE] + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state START to state MATCH */ 
        temp = EventSignalMat_SHATTER_SPECIAL(mat,i-1,j-1,START) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         SIG_0_0[MATCH] = score; 


        /* state MATCH is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > EventSignalMat_SHATTER_SPECIAL(mat,i,j,END) )     {  
          EventSignalMat_SHATTER_SPECIAL(mat,i,j,END) = temp;    
          }  




        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = SIG_0_1[MATCH] + mat->gap;   
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = SIG_0_1[INSERT_SIGNAL] + mat->gapext;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         SIG_0_0[INSERT_SIGNAL] = score; 


        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = SIG_1_0[MATCH] + mat->gap;   
        /* From state DELETE to state DELETE */ 
        temp = SIG_1_0[DELETE] + mat->gapext;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         SIG_0_0[DELETE] = score;    


        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = SIG_5_5[MATCH] + (mat->seqdiff_open+mat->seqdiff_ext);   
        /* From state MATCH to state DIFF_SEQ */ 
        temp = SIG_6_6[MATCH] + mat->seqdiff_open;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = SIG_1_0[DIFF_SEQ] + mat->seqdiff_ext;     
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = SIG_0_1[DIFF_SEQ] + mat->seqdiff_ext;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         SIG_0_0[DIFF_SEQ] = score;  


        /* Finished calculating state DIFF_SEQ */ 
        }  


      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      }  
    stop_reporting();    
    return TRUE;     
}    


/* Function:  search_EventSignalMat(dbsi,out,query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext)
 *
 * Descrip:    This function makes a database search of EventSignalMat
 *             It uses the dbsi structure to choose which implementation
 *             to use of the database searching. This way at run time you
 *             can switch between single threaded/multi-threaded or hardware
 *
 *
 * Arg:                dbsi [UNKN ] Undocumented argument [DBSearchImpl *]
 * Arg:                 out [UNKN ] Undocumented argument [Hscore *]
 * Arg:               query [UNKN ] Undocumented argument [SignalSeq*]
 * Arg:              target [UNKN ] Undocumented argument [Sequence*]
 * Arg:                  sm [UNKN ] Undocumented argument [SignalMap*]
 * Arg:                 gap [UNKN ] Undocumented argument [Score]
 * Arg:              gapext [UNKN ] Undocumented argument [Score]
 * Arg:        seqdiff_open [UNKN ] Undocumented argument [Score]
 * Arg:         seqdiff_ext [UNKN ] Undocumented argument [Score]
 *
 * Return [UNKN ]  Undocumented return value [Search_Return_Type]
 *
 */
Search_Return_Type search_EventSignalMat(DBSearchImpl * dbsi,Hscore * out,SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext) 
{
#ifdef PTHREAD   
    int i;   
    int thr_no;  
    pthread_attr_t pat;  
    struct thread_pool_holder_EventSignalMat * holder;   
#endif   
    if( out == NULL )    {  
      warn("Passed in a null Hscore object into search_EventSignalMat. Can't process results!"); 
      return SEARCH_ERROR;   
      }  
    if( dbsi == NULL )   {  
      warn("Passed in a null DBSearchImpl object into search_EventSignalMat. Can't process results!");   
      return SEARCH_ERROR;   
      }  
    if( dbsi->trace_level > 5 )  
      warn("Asking for trace level of %d in database search for EventSignalMat, but it was compiled with a trace level of 32. Not all trace statements can be shown",dbsi->trace_level); 
    switch(dbsi->type)   { /*switch on implementation*/ 
      case DBSearchImpl_Serial : 
        return serial_search_EventSignalMat(out,query,target ,sm,gap,gapext,seqdiff_open,seqdiff_ext);   
      case DBSearchImpl_Pthreads :   
#ifdef PTHREAD   
        holder = (struct thread_pool_holder_EventSignalMat *) ckalloc(sizeof(struct thread_pool_holder_EventSignalMat)); 
        if( holder == NULL )     {  
          warn("Unable to allocated thread pool datastructure...");  
          return SEARCH_ERROR;   
          }  
        holder->out = out;   
        holder->dbsi = dbsi; 
        holder->query = query;   
        holder->target = target; 
        holder->sm = sm; 
        holder->gap = gap;   
        holder->gapext = gapext; 
        holder->seqdiff_open = seqdiff_open; 
        holder->seqdiff_ext = seqdiff_ext;   
        if( pthread_mutex_init(&(holder->input_lock),NULL) != 0 )    
        fatal("Unable to iniated input mutex lock"); 
        if( pthread_mutex_init(&(holder->output_lock),NULL) != 0 )   
        fatal("Unable to iniated output mutex lock");    
        /* Let us rock! */ 
        thr_no = number_of_threads_DBSearchImpl(dbsi);   
        holder->pool = ckcalloc (thr_no,sizeof(pthread_t));  
        if( holder->pool == NULL )   {  
          warn("Unable to allocated thread pools");  
          return SEARCH_ERROR;   
          }  
        /* Build a thread attribute to make sure we get the most out of SMP boxes */ 
        pthread_attr_init(&pat);     
        /* Give thread libraries a hint that threads should be kernel threads */ 
#ifndef __sgi /* SGI can't set system scope ... */   
#ifdef  HAS_PTHREAD_SETSCOPE 
        pthread_attr_setscope(&pat, PTHREAD_SCOPE_SYSTEM);   
#endif /* set scope */   
#endif /* sgi */ 
        /* Give thread libraries a hint that there are num of threads to run */ 
#ifdef HAS_PTHREAD_SETCONCURRENCY    
        pthread_setconcurrency(thr_no+1);    
#endif /* set concurrency */ 
        for(i=0;i<thr_no;i++)    {  
          if( pthread_create(holder->pool+i,&pat,thread_loop_EventSignalMat,(void *)holder) )    
            fatal("Unable to create a thread!"); 
          }  
        /* Now - wait for all the threads to exit */ 
        for(i=0;i<thr_no;i++)    {  
          if( pthread_join(holder->pool[i],NULL) != 0 )  
            fatal("Unable to join a thread!");   
          }  
        /* Deallocate the thread structures */ 
        ckfree(holder->pool);    
        ckfree(holder);  
        return SEARCH_OK;    
#else /* not compiled with threads */    
        warn("You did not specifiy the PTHREAD compile when compiled the C code for EventSignalMat");    
#endif /* finished threads */    
      default :  
        warn("database search implementation %s was not provided in the compiled dynamite file from EventSignalMat",impl_string_DBSearchImpl(dbsi)); 
        return SEARCH_ERROR; 
      } /* end of switch on implementation */ 


}    


/* Function:  thread_loop_EventSignalMat(ptr)
 *
 * Descrip:    dummy loop code foreach thread for EventSignalMat
 *
 *
 * Arg:        ptr [UNKN ] Undocumented argument [void *]
 *
 * Return [UNKN ]  Undocumented return value [void *]
 *
 */
void * thread_loop_EventSignalMat(void * ptr) 
{
    fatal("dummy thread loop function"); 
}    


/* Function:  serial_search_EventSignalMat(out,query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext)
 *
 * Descrip:    This function makes a database search of EventSignalMat
 *             It is a single processor implementation
 *
 *
 * Arg:                 out [UNKN ] Undocumented argument [Hscore *]
 * Arg:               query [UNKN ] Undocumented argument [SignalSeq*]
 * Arg:              target [UNKN ] Undocumented argument [Sequence*]
 * Arg:                  sm [UNKN ] Undocumented argument [SignalMap*]
 * Arg:                 gap [UNKN ] Undocumented argument [Score]
 * Arg:              gapext [UNKN ] Undocumented argument [Score]
 * Arg:        seqdiff_open [UNKN ] Undocumented argument [Score]
 * Arg:         seqdiff_ext [UNKN ] Undocumented argument [Score]
 *
 * Return [UNKN ]  Undocumented return value [Search_Return_Type]
 *
 */
Search_Return_Type serial_search_EventSignalMat(Hscore * out,SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext) 
{
    int db_status;   
    int score;   
    int query_pos = 0;   
    int target_pos = 0;  
    DataScore * ds;  


    push_errormsg_stack("Before any actual search in db searching"); 


    target_pos = 0;  




    /* No maximum length - allocated on-the-fly */ 
    score = score_only_EventSignalMat(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext);   
    if( should_store_Hscore(out,score) == TRUE )     { /*if storing datascore*/ 
      ds = new_DataScore_from_storage(out);  
      if( ds == NULL )   {  
        warn("EventSignalMat search had a memory error in allocating a new_DataScore (?a leak somewhere - DataScore is a very small datastructure"); 
        return SEARCH_ERROR; 
        }  
      /* Now: add query/target information to the entry */ 
      ds->score = score;     
      add_Hscore(out,ds);    
      } /* end of if storing datascore */ 
    pop_errormsg_stack();    
    push_errormsg_stack("DB searching: just finished [Query Pos: %d] [Target Pos: %d]",query_pos,target_pos);    


    pop_errormsg_stack();    
    return SEARCH_OK;    
}    


/* Function:  score_only_EventSignalMat(query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext)
 *
 * Descrip:    This function just calculates the score for the matrix
 *             I am pretty sure we can do this better, but hey, for the moment...
 *             It calls /allocate_EventSignalMat_only
 *
 *
 * Arg:               query [UNKN ] query data structure [SignalSeq*]
 * Arg:              target [UNKN ] target data structure [Sequence*]
 * Arg:                  sm [UNKN ] Resource [SignalMap*]
 * Arg:                 gap [UNKN ] Resource [Score]
 * Arg:              gapext [UNKN ] Resource [Score]
 * Arg:        seqdiff_open [UNKN ] Resource [Score]
 * Arg:         seqdiff_ext [UNKN ] Resource [Score]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int score_only_EventSignalMat(SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext) 
{
    int bestscore = NEGI;    
    int i;   
    int j;   
    int k;   
    EventSignalMat * mat;    


    mat = allocate_EventSignalMat_only(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext);  
    if( mat == NULL )    {  
      warn("Memory allocation error in the db search - unable to communicate to calling function. this spells DIASTER!");    
      return NEGI;   
      }  
    if((mat->basematrix = BaseMatrix_alloc_matrix_and_specials(7,(mat->leni + 6) * 4,7,2)) == NULL)  {  
      warn("Score only matrix for EventSignalMat cannot be allocated, (asking for 6  by %d  cells)",mat->leni*4);    
      mat = free_EventSignalMat(mat);    
      return 0;  
      }  
    mat->basematrix->type = BASEMATRIX_TYPE_VERYSMALL;   


    /* Now, initiate matrix */ 
    for(j=0;j<8;j++) {  
      for(i=(-6);i<mat->leni;i++)    {  
        for(k=0;k<4;k++) 
          EventSignalMat_VSMALL_MATRIX(mat,i,j,k) = NEGI;    
        }  
      EventSignalMat_VSMALL_SPECIAL(mat,i,j,START) = 0;  
      EventSignalMat_VSMALL_SPECIAL(mat,i,j,END) = NEGI; 
      }  


    /* Ok, lets do-o-o-o-o it */ 


    for(j=0;j<mat->lenj;j++) { /*for all target positions*/ 
      auto int score;    
      auto int temp;     
      for(i=0;i<mat->leni;i++)   { /*for all query positions*/ 


        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;  
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-1,MATCH) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-1,DELETE) + 0;     
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state START to state MATCH */ 
        temp = EventSignalMat_VSMALL_SPECIAL(mat,i-1,j-1,START) + 0;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         EventSignalMat_VSMALL_MATRIX(mat,i,j,MATCH) = score;    


        /* state MATCH is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > EventSignalMat_VSMALL_SPECIAL(mat,i,j,END) )  {  
          EventSignalMat_VSMALL_SPECIAL(mat,i,j,END) = temp;     
          }  




        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_VSMALL_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;  
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_VSMALL_MATRIX(mat,i,j,INSERT_SIGNAL) = score;    


        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;  
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_VSMALL_MATRIX(mat,i,j,DELETE) = score;   


        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_VSMALL_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);  
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_VSMALL_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_VSMALL_MATRIX(mat,i,j,DIFF_SEQ) = score; 


        /* Finished calculating state DIFF_SEQ */ 
        } /* end of for all query positions */ 




      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      if( bestscore < EventSignalMat_VSMALL_SPECIAL(mat,0,j,END) )   
        bestscore = EventSignalMat_VSMALL_SPECIAL(mat,0,j,END);  
      } /* end of for all target positions */ 


    mat = free_EventSignalMat(mat);  
    return bestscore;    
}    


/* Function:  PackAln_bestmemory_EventSignalMat(query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext,dpenv,dpri)
 *
 * Descrip:    This function chooses the best memory set-up for the alignment
 *             using calls to basematrix, and then implements either a large
 *             or small memory model.
 *
 *             It is the best function to use if you just want an alignment
 *
 *             If you want a label alignment, you will need
 *             /convert_PackAln_to_AlnBlock_EventSignalMat
 *
 *
 * Arg:               query [UNKN ] query data structure [SignalSeq*]
 * Arg:              target [UNKN ] target data structure [Sequence*]
 * Arg:                  sm [UNKN ] Resource [SignalMap*]
 * Arg:                 gap [UNKN ] Resource [Score]
 * Arg:              gapext [UNKN ] Resource [Score]
 * Arg:        seqdiff_open [UNKN ] Resource [Score]
 * Arg:         seqdiff_ext [UNKN ] Resource [Score]
 * Arg:               dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:                dpri [UNKN ] Undocumented argument [DPRunImpl *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_bestmemory_EventSignalMat(SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext,DPEnvelope * dpenv,DPRunImpl * dpri) 
{
    long long total; 
    EventSignalMat * mat;    
    PackAln * out;   
    DebugMatrix * de;    
    DPRunImplMemory strategy;    
    assert(dpri);    


    total = query->len * target->len;    
    if( dpri->memory == DPIM_Default )   {  
      if( (total * 4 * sizeof(int)) > 1000*dpri->kbyte_size) {  
        strategy = DPIM_Linear;  
        }  
      else   {  
        strategy = DPIM_Explicit;    
        }  
      }  
    else {  
      strategy = dpri->memory;   
      }  


    if( dpenv != NULL )  {  
      if( strategy == DPIM_Explicit) {  
        if( (mat=allocate_Expl_EventSignalMat(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext,dpri)) == NULL )    {  
          warn("Unable to allocate large EventSignalMat version");   
          return NULL;   
          }  
        calculate_dpenv_EventSignalMat(mat,dpenv);   
        out =  PackAln_read_Expl_EventSignalMat(mat);    
        }  
      else   {  
        mat = allocate_EventSignalMat_only(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext);  
        calculate_shatter_EventSignalMat(mat,dpenv);     
        out = PackAln_read_Shatter_EventSignalMat(mat);  
        }  
      }  
    else {  
      if( strategy == DPIM_Linear )  {  
        /* use small implementation */ 
        if( (mat=allocate_Small_EventSignalMat(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext)) == NULL )    {  
          warn("Unable to allocate small EventSignalMat version");   
          return NULL;   
          }  
        out = PackAln_calculate_Small_EventSignalMat(mat,dpenv);     
        }  
      else   {  
        /* use Large implementation */ 
        if( (mat=allocate_Expl_EventSignalMat(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext,dpri)) == NULL )    {  
          warn("Unable to allocate large EventSignalMat version");   
          return NULL;   
          }  
        if( dpri->debug == TRUE) {  
          fatal("Asked for dydebug, but dynamite file not compiled with -g. Need to recompile dynamite source"); 
          }  
        else {  
          calculate_EventSignalMat(mat);     
          out =  PackAln_read_Expl_EventSignalMat(mat);  
          }  
        }  
      }  


    mat = free_EventSignalMat(mat);  
    return out;  
}    


/* Function:  allocate_EventSignalMat_only(query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext)
 *
 * Descrip:    This function only allocates the EventSignalMat structure
 *             checks types where possible and determines leni and lenj
 *             The basematrix area is delt with elsewhere
 *
 *
 * Arg:               query [UNKN ] query data structure [SignalSeq*]
 * Arg:              target [UNKN ] target data structure [Sequence*]
 * Arg:                  sm [UNKN ] Resource [SignalMap*]
 * Arg:                 gap [UNKN ] Resource [Score]
 * Arg:              gapext [UNKN ] Resource [Score]
 * Arg:        seqdiff_open [UNKN ] Resource [Score]
 * Arg:         seqdiff_ext [UNKN ] Resource [Score]
 *
 * Return [UNKN ]  Undocumented return value [EventSignalMat *]
 *
 */
EventSignalMat * allocate_EventSignalMat_only(SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext) 
{
    EventSignalMat * out;    


    if((out= EventSignalMat_alloc()) == NULL)    {  
      warn("Allocation of basic EventSignalMat structure failed...");    
      return NULL;   
      }  


    out->query = query;  
    out->target = target;    
    out->sm = sm;    
    out->gap = gap;  
    out->gapext = gapext;    
    out->seqdiff_open = seqdiff_open;    
    out->seqdiff_ext = seqdiff_ext;  
    out->leni = query->len;  
    out->lenj = target->len;     
    return out;  
}    


/* Function:  allocate_Expl_EventSignalMat(query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext,dpri)
 *
 * Descrip:    This function allocates the EventSignalMat structure
 *             and the basematrix area for explicit memory implementations
 *             It calls /allocate_EventSignalMat_only
 *
 *
 * Arg:               query [UNKN ] query data structure [SignalSeq*]
 * Arg:              target [UNKN ] target data structure [Sequence*]
 * Arg:                  sm [UNKN ] Resource [SignalMap*]
 * Arg:                 gap [UNKN ] Resource [Score]
 * Arg:              gapext [UNKN ] Resource [Score]
 * Arg:        seqdiff_open [UNKN ] Resource [Score]
 * Arg:         seqdiff_ext [UNKN ] Resource [Score]
 * Arg:                dpri [UNKN ] Undocumented argument [DPRunImpl *]
 *
 * Return [UNKN ]  Undocumented return value [EventSignalMat *]
 *
 */
EventSignalMat * allocate_Expl_EventSignalMat(SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext,DPRunImpl * dpri) 
{
    EventSignalMat * out;    


    out = allocate_EventSignalMat_only(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext);  
    if( out == NULL )    
      return NULL;   
    if( dpri->should_cache == TRUE ) {  
      if( dpri->cache != NULL )  {  
        if( dpri->cache->maxleni >= (out->lenj+6)*4 && dpri->cache->maxlenj >= (out->leni+6))    
          out->basematrix = hard_link_BaseMatrix(dpri->cache);   
        else 
          dpri->cache = free_BaseMatrix(dpri->cache);    
        }  
      }  
    if( out->basematrix == NULL )    {  
      if( (out->basematrix = BaseMatrix_alloc_matrix_and_specials((out->lenj+6)*4,(out->leni+6),2,out->lenj+6)) == NULL) {  
        warn("Explicit matrix EventSignalMat cannot be allocated, (asking for %d by %d main cells)",out->leni,out->lenj);    
        free_EventSignalMat(out);    
        return NULL; 
        }  
      }  
    if( dpri->should_cache == TRUE && dpri->cache == NULL)   
      dpri->cache = hard_link_BaseMatrix(out->basematrix);   
    out->basematrix->type = BASEMATRIX_TYPE_EXPLICIT;    
    init_EventSignalMat(out);    
    return out;  
}    


/* Function:  init_EventSignalMat(mat)
 *
 * Descrip:    This function initates EventSignalMat matrix when in explicit mode
 *             Called in /allocate_Expl_EventSignalMat
 *
 *
 * Arg:        mat [UNKN ] EventSignalMat which contains explicit basematrix memory [EventSignalMat *]
 *
 */
void init_EventSignalMat(EventSignalMat * mat) 
{
    register int i;  
    register int j;  
    if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT)   {  
      warn("Cannot iniate matrix, is not an explicit memory type and you have assummed that");   
      return;    
      }  


    for(i= (-6);i<mat->query->len;i++)   {  
      for(j= (-6);j<7;j++)   {  
        EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH) = NEGI;    
        EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;    
        EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE) = NEGI;   
        EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ) = NEGI; 
        }  
      }  
    for(j= (-6);j<mat->target->len;j++)  {  
      for(i= (-6);i<7;i++)   {  
        EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH) = NEGI;    
        EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;    
        EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE) = NEGI;   
        EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ) = NEGI; 
        }  
      EventSignalMat_EXPL_SPECIAL(mat,i,j,START) = 0;    
      EventSignalMat_EXPL_SPECIAL(mat,i,j,END) = NEGI;   
      }  
    return;  
}    


/* Function:  recalculate_PackAln_EventSignalMat(pal,mat)
 *
 * Descrip:    This function recalculates the PackAln structure produced by EventSignalMat
 *             For example, in linear space methods this is used to score them
 *
 *
 * Arg:        pal [UNKN ] Undocumented argument [PackAln *]
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 */
void recalculate_PackAln_EventSignalMat(PackAln * pal,EventSignalMat * mat) 
{
    int i,j,k,offi,offj; 
    PackAlnUnit * prev;  
    PackAlnUnit * pau;   


    for(k=1,prev=pal->pau[0];k < pal->len;k++,prev=pau)  {  
      pau = pal->pau[k]; 
      i = pau->i;    
      j = pau->j;    
      offi = pau->i - prev->i;   
      offj = pau->j - prev->j;   
      switch(pau->state) {  
        case MATCH :     
          if( offi == 1 && offj == 1 && prev->state == DIFF_SEQ )    {  
            pau->score = 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));   
            continue;    
            }  
          if( offi == 1 && offj == 1 && prev->state == MATCH )   {  
            pau->score = 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));   
            continue;    
            }  
          if( offi == 1 && offj == 1 && prev->state == INSERT_SIGNAL )   {  
            pau->score = 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));   
            continue;    
            }  
          if( offi == 1 && offj == 1 && prev->state == DELETE )  {  
            pau->score = 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));   
            continue;    
            }  
          if( offj == 1 && prev->state == (START+4) )    {  
            pau->score = 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));   
            continue;    
            }  
          warn("In recaluclating PackAln with state MATCH, from [%d,%d,%d], got a bad source state. Error!",offi,offj,prev->state);  
          break; 
        case INSERT_SIGNAL :     
          if( offi == 0 && offj == 1 && prev->state == MATCH )   {  
            pau->score = mat->gap + (0);     
            continue;    
            }  
          if( offi == 0 && offj == 1 && prev->state == INSERT_SIGNAL )   {  
            pau->score = mat->gapext + (0);  
            continue;    
            }  
          warn("In recaluclating PackAln with state INSERT_SIGNAL, from [%d,%d,%d], got a bad source state. Error!",offi,offj,prev->state);  
          break; 
        case DELETE :    
          if( offi == 1 && offj == 0 && prev->state == MATCH )   {  
            pau->score = mat->gap + (0);     
            continue;    
            }  
          if( offi == 1 && offj == 0 && prev->state == DELETE )  {  
            pau->score = mat->gapext + (0);  
            continue;    
            }  
          warn("In recaluclating PackAln with state DELETE, from [%d,%d,%d], got a bad source state. Error!",offi,offj,prev->state); 
          break; 
        case DIFF_SEQ :  
          if( offi == 5 && offj == 5 && prev->state == MATCH )   {  
            pau->score = (mat->seqdiff_open+mat->seqdiff_ext) + (0);     
            continue;    
            }  
          if( offi == 6 && offj == 6 && prev->state == MATCH )   {  
            pau->score = mat->seqdiff_open + (0);    
            continue;    
            }  
          if( offi == 1 && offj == 0 && prev->state == DIFF_SEQ )    {  
            pau->score = mat->seqdiff_ext + (0);     
            continue;    
            }  
          if( offi == 0 && offj == 1 && prev->state == DIFF_SEQ )    {  
            pau->score = mat->seqdiff_ext + (0);     
            continue;    
            }  
          warn("In recaluclating PackAln with state DIFF_SEQ, from [%d,%d,%d], got a bad source state. Error!",offi,offj,prev->state);   
          break; 
        case (START+4) :     
          warn("In recaluclating PackAln with state START, got a bad source state. Error!"); 
          break; 
        case (END+4) :   
          if( offj == 0 && prev->state == MATCH )    {  
            /* i here comes from the previous state ;) - not the real one */ 
            i = prev->i; 
            pau->score = 0 + (0);    
            continue;    
            }  
          warn("In recaluclating PackAln with state END, got a bad source state. Error!");   
          break; 
        default :    
          warn("In recaluclating PackAln got a bad recipient state. Error!");    
        }  
      prev = pau;    
      }  
    return;  
}    
/* divide and conquor macros are next */ 
#define EventSignalMat_HIDDEN_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[(j-hiddenj+6)][(i+6)*4+state])    
#define EventSignalMat_DC_SHADOW_MATRIX(thismatrix,i,j,state) (thismatrix->basematrix->matrix[((j+7)*8) % 56][(i+6)*4+state])    
#define EventSignalMat_HIDDEN_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state][(j+6)])   
#define EventSignalMat_DC_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*8][(j+6)])  
#define EventSignalMat_DC_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->matrix[((((j+7)*8)+(shadow+1)) % 56)][(i+6)*4 + state]) 
#define EventSignalMat_DC_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*8 +shadow+1][(j+6)])  
#define EventSignalMat_DC_OPT_SHADOW_MATRIX(thismatrix,i,j,state) (score_pointers[(((j+6)% 6) * (leni+1) * 4) + ((i+6) * 4) + (state)])  
#define EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(thismatrix,i,j,state,shadow) (shadow_pointers[(((j+6)% 6) * (leni+1) * 32) + ((i+6) * 32) + (state * 8) + shadow+1])  
#define EventSignalMat_DC_OPT_SHADOW_SPECIAL(thismatrix,i,j,state) (thismatrix->basematrix->specmatrix[state*8][(j+6)])  
/* Function:  allocate_Small_EventSignalMat(query,target,sm,gap,gapext,seqdiff_open,seqdiff_ext)
 *
 * Descrip:    This function allocates the EventSignalMat structure
 *             and the basematrix area for a small memory implementations
 *             It calls /allocate_EventSignalMat_only
 *
 *
 * Arg:               query [UNKN ] query data structure [SignalSeq*]
 * Arg:              target [UNKN ] target data structure [Sequence*]
 * Arg:                  sm [UNKN ] Resource [SignalMap*]
 * Arg:                 gap [UNKN ] Resource [Score]
 * Arg:              gapext [UNKN ] Resource [Score]
 * Arg:        seqdiff_open [UNKN ] Resource [Score]
 * Arg:         seqdiff_ext [UNKN ] Resource [Score]
 *
 * Return [UNKN ]  Undocumented return value [EventSignalMat *]
 *
 */
#define EventSignalMat_DC_OPT_SHADOW_SPECIAL_SP(thismatrix,i,j,state,shadow) (thismatrix->basematrix->specmatrix[state*8 +shadow+1][(j+6)])  
EventSignalMat * allocate_Small_EventSignalMat(SignalSeq* query,Sequence* target ,SignalMap* sm,Score gap,Score gapext,Score seqdiff_open,Score seqdiff_ext) 
{
    EventSignalMat * out;    


    out = allocate_EventSignalMat_only(query, target , sm, gap, gapext, seqdiff_open, seqdiff_ext);  
    if( out == NULL )    
      return NULL;   
    out->basematrix = BaseMatrix_alloc_matrix_and_specials(56,(out->leni + 6) * 4,16,out->lenj+6);   
    if(out == NULL)  {  
      warn("Small shadow matrix EventSignalMat cannot be allocated, (asking for 7 by %d main cells)",out->leni+7);   
      free_EventSignalMat(out);  
      return NULL;   
      }  
    out->basematrix->type = BASEMATRIX_TYPE_SHADOW;  
    return out;  
}    


/* Function:  PackAln_calculate_Small_EventSignalMat(mat,dpenv)
 *
 * Descrip:    This function calculates an alignment for EventSignalMat structure in linear space
 *             If you want only the start/end points
 *             use /AlnRangeSet_calculate_Small_EventSignalMat 
 *
 *             The function basically
 *               finds start/end points 
 *               foreach start/end point 
 *                 calls /full_dc_EventSignalMat 
 *
 *
 * Arg:          mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_calculate_Small_EventSignalMat(EventSignalMat * mat,DPEnvelope * dpenv) 
{
    int endj;    
    int score;   
    PackAln * out;   
    PackAlnUnit * pau;   
    int starti;  
    int startj;  
    int startstate;  
    int stopi;   
    int stopj;   
    int stopstate;   
    int temp;    
    int donej;  /* This is for reporting, will be passed as a & arg in */ 
    int totalj; /* This also is for reporting, but as is not changed, can be passed by value */ 


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW )    {  
      warn("Could not calculate packaln small for EventSignalMat due to wrong type of matrix");  
      return NULL;   
      }  


    out = PackAln_alloc_std();   


    start_reporting("Find start end points: ");  
    dc_optimised_start_end_calc_EventSignalMat(mat,dpenv);   
    score = start_end_find_end_EventSignalMat(mat,&endj);    
    out->score = score;  
    stopstate = END;
    
    /* No special to specials: one matrix alignment: simply remove and get */ 
    starti = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,0);  
    startj = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,1);  
    startstate = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,2);  
    stopi = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,3);   
    stopj = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,4);   
    stopstate = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,5);   
    temp = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,endj,END,6);    
    log_full_error(REPORT,0,"[%d,%d][%d,%d] Score %d",starti,startj,stopi,stopj,score);  
    stop_reporting();    
    start_reporting("Recovering alignment: ");   


    /* Figuring how much j we have to align for reporting purposes */ 
    donej = 0;   
    totalj = stopj - startj; 
    full_dc_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,out,&donej,totalj,dpenv);  


    /* Although we have no specials, need to get start. Better to check than assume */ 


    max_matrix_to_special_EventSignalMat(mat,starti,startj,startstate,temp,&stopi,&stopj,&stopstate,&temp,NULL); 
    if( stopi == EventSignalMat_READ_OFF_ERROR || stopstate != START )   {  
      warn("Problem in reading off special state system, hit a non start state (or an internal error) in a single alignment mode");  
      invert_PackAln(out);   
      recalculate_PackAln_EventSignalMat(out,mat);   
      return out;    
      }  


    /* Ok. Put away start start... */ 
    pau = PackAlnUnit_alloc();   
    pau->i = stopi;  
    pau->j = stopj;  
    pau->state = stopstate + 4;  
    add_PackAln(out,pau);    


    log_full_error(REPORT,0,"Alignment recovered");  
    stop_reporting();    
    invert_PackAln(out); 
    recalculate_PackAln_EventSignalMat(out,mat); 
    return out;  


}    


/* Function:  AlnRangeSet_calculate_Small_EventSignalMat(mat)
 *
 * Descrip:    This function calculates an alignment for EventSignalMat structure in linear space
 *             If you want the full alignment, use /PackAln_calculate_Small_EventSignalMat 
 *             If you have already got the full alignment, but want the range set, use /AlnRangeSet_from_PackAln_EventSignalMat
 *             If you have got the small matrix but not the alignment, use /AlnRangeSet_from_EventSignalMat 
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 * Return [UNKN ]  Undocumented return value [AlnRangeSet *]
 *
 */
AlnRangeSet * AlnRangeSet_calculate_Small_EventSignalMat(EventSignalMat * mat) 
{
    AlnRangeSet * out;   


    start_reporting("Find start end points: ");  
    dc_optimised_start_end_calc_EventSignalMat(mat,NULL);    
    log_full_error(REPORT,0,"Calculated");   


    out = AlnRangeSet_from_EventSignalMat(mat);  
    return out;  
}    


/* Function:  AlnRangeSet_from_EventSignalMat(mat)
 *
 * Descrip:    This function reads off a start/end structure
 *             for EventSignalMat structure in linear space
 *             If you want the full alignment use
 *             /PackAln_calculate_Small_EventSignalMat 
 *             If you have not calculated the matrix use
 *             /AlnRange_calculate_Small_EventSignalMat
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 * Return [UNKN ]  Undocumented return value [AlnRangeSet *]
 *
 */
AlnRangeSet * AlnRangeSet_from_EventSignalMat(EventSignalMat * mat) 
{
    AlnRangeSet * out;   
    AlnRange * temp; 
    int jpos;    
    int state;   


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW) {  
      warn("Bad error! - non shadow matrix type in AlnRangeSet_from_EventSignalMat");    
      return NULL;   
      }  


    out = AlnRangeSet_alloc_std();   
    /* Find the end position */ 
    out->score = start_end_find_end_EventSignalMat(mat,&jpos);   
    state = END; 


    while( (temp = AlnRange_build_EventSignalMat(mat,jpos,state,&jpos,&state)) != NULL)  
      add_AlnRangeSet(out,temp); 
    return out;  
}    


/* Function:  AlnRange_build_EventSignalMat(mat,stopj,stopspecstate,startj,startspecstate)
 *
 * Descrip:    This function calculates a single start/end set in linear space
 *             Really a sub-routine for /AlnRangeSet_from_PackAln_EventSignalMat
 *
 *
 * Arg:                   mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:                 stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopspecstate [UNKN ] Undocumented argument [int]
 * Arg:                startj [UNKN ] Undocumented argument [int *]
 * Arg:        startspecstate [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [AlnRange *]
 *
 */
AlnRange * AlnRange_build_EventSignalMat(EventSignalMat * mat,int stopj,int stopspecstate,int * startj,int * startspecstate) 
{
    AlnRange * out;  
    int jpos;    
    int state;   


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW) {  
      warn("Bad error! - non shadow matrix type in AlnRangeSet_from_EventSignalMat");    
      return NULL;   
      }  


    /* Assumme that we have specials (we should!). Read back along the specials till we have the finish point */ 
    if( read_special_strip_EventSignalMat(mat,0,stopj,stopspecstate,&jpos,&state,NULL) == FALSE) {  
      warn("In AlnRanger_build_EventSignalMat alignment ending at %d, unable to read back specials. Will (evenutally) return a partial range set... BEWARE!",stopj); 
      return NULL;   
      }  
    if( state == START || jpos <= 0) 
      return NULL;   


    out = AlnRange_alloc();  


    out->starti = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,0);   
    out->startj = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,1);   
    out->startstate = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,2);   
    out->stopi = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,3);    
    out->stopj = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,4);    
    out->stopstate = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,5);    
    out->startscore = EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,jpos,state,6);   
    out->stopscore = EventSignalMat_DC_SHADOW_SPECIAL(mat,0,jpos,state); 


    /* Now, we have to figure out where this state came from in the specials */ 
    max_matrix_to_special_EventSignalMat(mat,out->starti,out->startj,out->startstate,out->startscore,&jpos,startj,startspecstate,&state,NULL);   
    if( jpos == EventSignalMat_READ_OFF_ERROR)   {  
      warn("In AlnRange_build_EventSignalMat alignment ending at %d, with aln range between %d-%d in j, unable to find source special, returning this range, but this could get tricky!",stopj,out->startj,out->stopj);  
      return out;    
      }  


    /* Put in the correct score for startstate, from the special */ 
    out->startscore = EventSignalMat_DC_SHADOW_SPECIAL(mat,0,*startj,*startspecstate);   
    /* The correct j coords have been put into startj, startspecstate... so just return out */ 
    return out;  
}    


/* Function:  read_hidden_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,out)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            starti [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int]
 * Arg:        startstate [UNKN ] Undocumented argument [int]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:               out [UNKN ] Undocumented argument [PackAln *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean read_hidden_EventSignalMat(EventSignalMat * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,PackAln * out) 
{
    int i;   
    int j;   
    int state;   
    int cellscore;   
    int isspecial;   
    /* We don't need hiddenj here, 'cause matrix access handled by max funcs */ 
    PackAlnUnit * pau;   


    /* stop position is on the path */ 
    i = stopi;   
    j = stopj;   
    state= stopstate;    
    isspecial = FALSE;   


    while( i >= starti && j >= startj)   {  
      /* Put away current i,j,state */ 
      pau = PackAlnUnit_alloc();/* Should deal with memory overflow */ 
      pau->i = i;    
      pau->j = j;    
      pau->state =  state;   
      add_PackAln(out,pau);  


      max_hidden_EventSignalMat(mat,startj,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore);  


      if( i == EventSignalMat_READ_OFF_ERROR)    {  
        warn("In EventSignalMat hidden read off, between %d:%d,%d:%d - at got bad read off. Problem!",starti,startj,stopi,stopj);    
        return FALSE;    
        }  


      if( i == starti && j == startj && state == startstate) {  
/* Put away final state (start of this block) */ 
        pau = PackAlnUnit_alloc();  /* Should deal with memory overflow */ 
        pau->i = i;  
        pau->j = j;  
        pau->state =  state; 
        add_PackAln(out,pau);    
          return TRUE;   
        }  
      if( i == starti && j == startj)    {  
        warn("In EventSignalMat hidden read off, between %d:%d,%d:%d - hit start cell, but not in start state. Can't be good!.",starti,startj,stopi,stopj);  
        return FALSE;    
        }  
      }  
    warn("In EventSignalMat hidden read off, between %d:%d,%d:%d - gone past start cell (now in %d,%d,%d), can't be good news!.",starti,startj,stopi,stopj,i,j,state);   
    return FALSE;    
}    


/* Function:  max_hidden_EventSignalMat(mat,hiddenj,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:           hiddenj [UNKN ] Undocumented argument [int]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_hidden_EventSignalMat(EventSignalMat * mat,int hiddenj,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore) 
{
    register int temp;   
    register int cscore; 


    *reti = (*retj) = (*retstate) = EventSignalMat_READ_OFF_ERROR;   


    if( i < 0 || j < 0 || i > mat->query->len || j > mat->target->len)   {  
      warn("In EventSignalMat matrix special read off - out of bounds on matrix [i,j is %d,%d state %d in standard matrix]",i,j,state);  
      return -1; 
      }  


    /* Then you have to select the correct switch statement to figure out the readoff      */ 
    /* Somewhat odd - reverse the order of calculation and return as soon as it is correct */ 
    cscore = EventSignalMat_HIDDEN_MATRIX(mat,i,j,state);    
    switch(state)    { /*Switch state */ 
      case MATCH :   
        /* Not allowing special sources.. skipping START */ 
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,DELETE) )   {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = DELETE;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,DELETE);  
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,DELETE);   
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,INSERT_SIGNAL) )    {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = INSERT_SIGNAL; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,INSERT_SIGNAL);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,INSERT_SIGNAL);    
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,MATCH) )    {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,MATCH);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,MATCH);    
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,DIFF_SEQ) ) {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = DIFF_SEQ;  
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,DIFF_SEQ);    
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 1,DIFF_SEQ);     
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      case INSERT_SIGNAL :   
        temp = cscore - (mat->gapext) -  (0);    
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 0,j - 1,INSERT_SIGNAL) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = INSERT_SIGNAL; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-0,j-1,INSERT_SIGNAL);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 0,j - 1,INSERT_SIGNAL);    
          }  
        temp = cscore - (mat->gap) -  (0);   
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 0,j - 1,MATCH) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-0,j-1,MATCH);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 0,j - 1,MATCH);    
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      case DELETE :  
        temp = cscore - (mat->gapext) -  (0);    
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 0,DELETE) )   {  
          *reti = i - 1; 
          *retj = j - 0; 
          *retstate = DELETE;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-0,DELETE);  
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 0,DELETE);   
          }  
        temp = cscore - (mat->gap) -  (0);   
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 0,MATCH) )    {  
          *reti = i - 1; 
          *retj = j - 0; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-0,MATCH);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 0,MATCH);    
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      case DIFF_SEQ :    
        temp = cscore - (mat->seqdiff_ext) -  (0);   
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 0,j - 1,DIFF_SEQ) ) {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = DIFF_SEQ;  
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-0,j-1,DIFF_SEQ);    
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 0,j - 1,DIFF_SEQ);     
          }  
        temp = cscore - (mat->seqdiff_ext) -  (0);   
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 0,DIFF_SEQ) ) {  
          *reti = i - 1; 
          *retj = j - 0; 
          *retstate = DIFF_SEQ;  
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-0,DIFF_SEQ);    
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 1,j - 0,DIFF_SEQ);     
          }  
        temp = cscore - (mat->seqdiff_open) -  (0);  
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 6,j - 6,MATCH) )    {  
          *reti = i - 6; 
          *retj = j - 6; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-6,j-6,MATCH);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 6,j - 6,MATCH);    
          }  
        temp = cscore - ((mat->seqdiff_open+mat->seqdiff_ext)) -  (0);   
        if( temp == EventSignalMat_HIDDEN_MATRIX(mat,i - 5,j - 5,MATCH) )    {  
          *reti = i - 5; 
          *retj = j - 5; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_HIDDEN_MATRIX(mat,i-5,j-5,MATCH);   
            }  
          return EventSignalMat_HIDDEN_MATRIX(mat,i - 5,j - 5,MATCH);    
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      default:   
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      } /* end of Switch state  */ 
}    


/* Function:  read_special_strip_EventSignalMat(mat,stopi,stopj,stopstate,startj,startstate,out)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int *]
 * Arg:        startstate [UNKN ] Undocumented argument [int *]
 * Arg:               out [UNKN ] Undocumented argument [PackAln *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean read_special_strip_EventSignalMat(EventSignalMat * mat,int stopi,int stopj,int stopstate,int * startj,int * startstate,PackAln * out) 
{
    int i;   
    int j;   
    int state;   
    int cellscore;   
    int isspecial;   
    PackAlnUnit * pau;   


    /* stop position is on the path */ 
    i = stopi;   
    j = stopj;   
    state= stopstate;    
    isspecial = TRUE;    


    /* Loop until state has the same j as its stop in shadow pointers */ 
    /* This will be the state is came out from, OR it has hit !start */ 
    /* We may not want to get the alignment, in which case out will be NULL */ 
    while( j > EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,i,j,state,4) && state != START)   { /*while more specials to eat up*/ 
      /* Put away current state, if we should */ 
      if(out != NULL)    {  
        pau = PackAlnUnit_alloc();  /* Should deal with memory overflow */ 
        pau->i = i;  
        pau->j = j;  
        pau->state =  state + 4; 
        add_PackAln(out,pau);    
        }  


      max_special_strip_EventSignalMat(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore);  
      if( i == EventSignalMat_READ_OFF_ERROR)    {  
        warn("In special strip read EventSignalMat, got a bad read off error. Sorry!");  
        return FALSE;    
        }  
      } /* end of while more specials to eat up */ 


    /* check to see we have not gone too far! */ 
    if( state != START && j < EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,i,j,state,4))  {  
      warn("In special strip read EventSignalMat, at special [%d] state [%d] overshot!",j,state);    
      return FALSE;  
      }  
    /* Put away last state */ 
    if(out != NULL)  {  
      pau = PackAlnUnit_alloc();/* Should deal with memory overflow */ 
      pau->i = i;    
      pau->j = j;    
      pau->state =  state + 4;   
      add_PackAln(out,pau);  
      }  


    /* Put away where we are in startj and startstate */ 
    *startj = j; 
    *startstate = state; 
    return TRUE; 
}    


/* Function:  max_special_strip_EventSignalMat(mat,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore)
 *
 * Descrip:    A pretty intense internal function. Deals with read-off only in specials
 *
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_special_strip_EventSignalMat(EventSignalMat * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore) 
{
    int temp;    
    int cscore;  


    *reti = (*retj) = (*retstate) = EventSignalMat_READ_OFF_ERROR;   
    if( isspecial == FALSE ) {  
      warn("In special strip max function for EventSignalMat, got a non special start point. Problem! (bad!)");  
      return (-1);   
      }  


    if( j < 0 || j > mat->target->len)   {  
      warn("In EventSignalMat matrix special read off - out of bounds on matrix [j is %d in special]",j);    
      return -1; 
      }  


    cscore = EventSignalMat_DC_SHADOW_SPECIAL(mat,i,j,state);    
    switch(state)    { /*switch on special states*/ 
      case START :   
      case END :     
        /* Source MATCH is not a special */ 
      default:   
        warn("Major problem (!) - in EventSignalMat special strip read off, position %d,%d state %d no source found  dropped into default on source switch!",i,j,state); 
        return (-1); 
      } /* end of switch on special states */ 
}    


/* Function:  max_matrix_to_special_EventSignalMat(mat,i,j,state,cscore,reti,retj,retstate,retspecial,cellscore)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:            cscore [UNKN ] Undocumented argument [int]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_matrix_to_special_EventSignalMat(EventSignalMat * mat,int i,int j,int state,int cscore,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore) 
{
    int temp;    
    *reti = (*retj) = (*retstate) = EventSignalMat_READ_OFF_ERROR;   


    if( j < 0 || j > mat->lenj)  {  
      warn("In EventSignalMat matrix to special read off - out of bounds on matrix [j is %d in special]",j); 
      return -1; 
      }  


    switch(state)    { /*Switch state */ 
      case MATCH :   
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));     
        if( temp == EventSignalMat_DC_SHADOW_SPECIAL(mat,i - 1,j - 1,START) )    {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = START; 
          *retspecial = TRUE;    
          if( cellscore != NULL) {  
            *cellscore = cscore - EventSignalMat_DC_SHADOW_SPECIAL(mat,i-1,j-1,START);   
            }  
          return EventSignalMat_DC_SHADOW_MATRIX(mat,i - 1,j - 1,START) ;    
          }  
        /* Source DELETE is not a special, should not get here! */ 
        /* Source INSERT_SIGNAL is not a special, should not get here! */ 
        /* Source MATCH is not a special, should not get here! */ 
        /* Source DIFF_SEQ is not a special, should not get here! */ 
        warn("Major problem (!) - in EventSignalMat matrix to special read off, position %d,%d state %d no source found!",i,j,state);    
        return (-1); 
      case INSERT_SIGNAL :   
        /* Source INSERT_SIGNAL is not a special, should not get here! */ 
        /* Source MATCH is not a special, should not get here! */ 
        warn("Major problem (!) - in EventSignalMat matrix to special read off, position %d,%d state %d no source found!",i,j,state);    
        return (-1); 
      case DELETE :  
        /* Source DELETE is not a special, should not get here! */ 
        /* Source MATCH is not a special, should not get here! */ 
        warn("Major problem (!) - in EventSignalMat matrix to special read off, position %d,%d state %d no source found!",i,j,state);    
        return (-1); 
      case DIFF_SEQ :    
        /* Source DIFF_SEQ is not a special, should not get here! */ 
        /* Source DIFF_SEQ is not a special, should not get here! */ 
        /* Source MATCH is not a special, should not get here! */ 
        /* Source MATCH is not a special, should not get here! */ 
        warn("Major problem (!) - in EventSignalMat matrix to special read off, position %d,%d state %d no source found!",i,j,state);    
        return (-1); 
      default:   
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      } /* end of Switch state  */ 


}    


/* Function:  calculate_hidden_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            starti [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int]
 * Arg:        startstate [UNKN ] Undocumented argument [int]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:             dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 */
void calculate_hidden_EventSignalMat(EventSignalMat * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,DPEnvelope * dpenv) 
{
    register int i;  
    register int j;  
    register int score;  
    register int temp;   
    register int hiddenj;    


    hiddenj = startj;    


    init_hidden_EventSignalMat(mat,starti,startj,stopi,stopj);   


    EventSignalMat_HIDDEN_MATRIX(mat,starti,startj,startstate) = 0;  


    for(j=startj;j<=stopj;j++)   {  
      for(i=starti;i<=stopi;i++) {  
        /* Should *not* do very first cell as this is the one set to zero in one state! */ 
        if( i == starti && j == startj ) 
          continue;  
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          EventSignalMat_HIDDEN_MATRIX(mat,i,j,MATCH) = NEGI;    
          EventSignalMat_HIDDEN_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;    
          EventSignalMat_HIDDEN_MATRIX(mat,i,j,DELETE) = NEGI;   
          EventSignalMat_HIDDEN_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;     
          continue;  
          } /* end of Is not in envelope */ 


        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;  
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,MATCH) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-1,DELETE) + 0;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         EventSignalMat_HIDDEN_MATRIX(mat,i,j,MATCH) = score;    
        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_HIDDEN_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;  
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_HIDDEN_MATRIX(mat,i,j,INSERT_SIGNAL) = score;    
        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;  
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_HIDDEN_MATRIX(mat,i,j,DELETE) = score;   
        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_HIDDEN_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);  
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_HIDDEN_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_HIDDEN_MATRIX(mat,i,j,DIFF_SEQ) = score; 
        /* Finished calculating state DIFF_SEQ */ 
        }  
      }  


    return;  
}    


/* Function:  init_hidden_EventSignalMat(mat,starti,startj,stopi,stopj)
 *
 * Descrip: No Description
 *
 * Arg:           mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:        starti [UNKN ] Undocumented argument [int]
 * Arg:        startj [UNKN ] Undocumented argument [int]
 * Arg:         stopi [UNKN ] Undocumented argument [int]
 * Arg:         stopj [UNKN ] Undocumented argument [int]
 *
 */
void init_hidden_EventSignalMat(EventSignalMat * mat,int starti,int startj,int stopi,int stopj) 
{
    register int i;  
    register int j;  
    register int hiddenj;    


    hiddenj = startj;    
    for(j=(startj-6);j<=stopj;j++)   {  
      for(i=(starti-6);i<=stopi;i++) {  
        EventSignalMat_HIDDEN_MATRIX(mat,i,j,MATCH) = NEGI;
 
        EventSignalMat_HIDDEN_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;
 
        EventSignalMat_HIDDEN_MATRIX(mat,i,j,DELETE) = NEGI;
    
        EventSignalMat_HIDDEN_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;
  
        }  
      }  


    return;  
}    


/* Function:  full_dc_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,out,donej,totalj,dpenv)
 *
 * Descrip:    The main divide-and-conquor routine. Basically, call /PackAln_calculate_small_EventSignalMat
 *             Not this function, which is pretty hard core. 
 *             Function is given start/end points (in main matrix) for alignment
 *             It does some checks, decides whether start/end in j is small enough for explicit calc
 *               - if yes, calculates it, reads off into PackAln (out), adds the j distance to donej and returns TRUE
 *               - if no,  uses /do_dc_single_pass_EventSignalMat to get mid-point
 *                          saves midpoint, and calls itself to do right portion then left portion
 *             right then left ensures PackAln is added the 'right' way, ie, back-to-front
 *             returns FALSE on any error, with a warning
 *
 *
 * Arg:               mat [UNKN ] Matrix with small memory implementation [EventSignalMat *]
 * Arg:            starti [UNKN ] Start position in i [int]
 * Arg:            startj [UNKN ] Start position in j [int]
 * Arg:        startstate [UNKN ] Start position state number [int]
 * Arg:             stopi [UNKN ] Stop position in i [int]
 * Arg:             stopj [UNKN ] Stop position in j [int]
 * Arg:         stopstate [UNKN ] Stop position state number [int]
 * Arg:               out [UNKN ] PackAln structure to put alignment into [PackAln *]
 * Arg:             donej [UNKN ] pointer to a number with the amount of alignment done [int *]
 * Arg:            totalj [UNKN ] total amount of alignment to do (in j coordinates) [int]
 * Arg:             dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean full_dc_EventSignalMat(EventSignalMat * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,PackAln * out,int * donej,int totalj,DPEnvelope * dpenv) 
{
    int lstarti; 
    int lstartj; 
    int lstate;  


    if( mat->basematrix->type != BASEMATRIX_TYPE_SHADOW) {  
      warn("*Very* bad error! - non shadow matrix type in full_dc_EventSignalMat");  
      return FALSE;  
      }  


    if( starti == -1 || startj == -1 || startstate == -1 || stopi == -1 || stopstate == -1)  {  
      warn("In full dc program, passed bad indices, indices passed were %d:%d[%d] to %d:%d[%d]\n",starti,startj,startstate,stopi,stopj,stopstate);   
      return FALSE;  
      }  


    if( stopj - startj < 30) {  
      log_full_error(REPORT,0,"[%d,%d][%d,%d] Explicit read off",starti,startj,stopi,stopj);/* Build hidden explicit matrix */ 
      calculate_hidden_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv);     
      *donej += (stopj - startj);   /* Now read it off into out */ 
      if( read_hidden_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,out) == FALSE)   {  
        warn("In full dc, at %d:%d,%d:%d got a bad hidden explicit read off... ",starti,startj,stopi,stopj); 
        return FALSE;    
        }  
      return TRUE;   
      }  


/* In actual divide and conquor */ 
    if( do_dc_single_pass_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv,(int)(*donej*100)/totalj) == FALSE)    {  
      warn("In divide and conquor for EventSignalMat, at bound %d:%d to %d:%d, unable to calculate midpoint. Problem!",starti,startj,stopi,stopj);   
      return FALSE;  
      }  


/* Ok... now we have to call on each side of the matrix */ 
/* We have to retrieve left hand side positions, as they will be vapped by the time we call LHS */ 
    lstarti= EventSignalMat_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,0);    
    lstartj= EventSignalMat_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,1);    
    lstate = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,2);    


/* Call on right hand side: this lets us do the correct read off */ 
    if( full_dc_EventSignalMat(mat,EventSignalMat_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,3),EventSignalMat_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,4),EventSignalMat_DC_SHADOW_MATRIX_SP(mat,stopi,stopj,stopstate,5),stopi,stopj,stopstate,out,donej,totalj,dpenv) == FALSE)   {  
/* Warning already issued, simply chained back up to top */ 
      return FALSE;  
      }  
/* Call on left hand side */ 
    if( full_dc_EventSignalMat(mat,starti,startj,startstate,lstarti,lstartj,lstate,out,donej,totalj,dpenv) == FALSE) {  
/* Warning already issued, simply chained back up to top */ 
      return FALSE;  
      }  


    return TRUE;     
}    


/* Function:  do_dc_single_pass_EventSignalMat(mat,starti,startj,startstate,stopi,stopj,stopstate,dpenv,perc_done)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            starti [UNKN ] Undocumented argument [int]
 * Arg:            startj [UNKN ] Undocumented argument [int]
 * Arg:        startstate [UNKN ] Undocumented argument [int]
 * Arg:             stopi [UNKN ] Undocumented argument [int]
 * Arg:             stopj [UNKN ] Undocumented argument [int]
 * Arg:         stopstate [UNKN ] Undocumented argument [int]
 * Arg:             dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:         perc_done [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean do_dc_single_pass_EventSignalMat(EventSignalMat * mat,int starti,int startj,int startstate,int stopi,int stopj,int stopstate,DPEnvelope * dpenv,int perc_done) 
{
    int halfj;   
    halfj = startj + ((stopj - startj)/2);   


    init_dc_EventSignalMat(mat); 


    EventSignalMat_DC_SHADOW_MATRIX(mat,starti,startj,startstate) = 0;   
    run_up_dc_EventSignalMat(mat,starti,stopi,startj,halfj-1,dpenv,perc_done);   
    push_dc_at_merge_EventSignalMat(mat,starti,stopi,halfj,&halfj,dpenv);    
    follow_on_dc_EventSignalMat(mat,starti,stopi,halfj,stopj,dpenv,perc_done);   
    return TRUE; 
}    


/* Function:  push_dc_at_merge_EventSignalMat(mat,starti,stopi,startj,stopj,dpenv)
 *
 * Descrip: No Description
 *
 * Arg:           mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:        starti [UNKN ] Undocumented argument [int]
 * Arg:         stopi [UNKN ] Undocumented argument [int]
 * Arg:        startj [UNKN ] Undocumented argument [int]
 * Arg:         stopj [UNKN ] Undocumented argument [int *]
 * Arg:         dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 */
void push_dc_at_merge_EventSignalMat(EventSignalMat * mat,int starti,int stopi,int startj,int * stopj,DPEnvelope * dpenv) 
{
    register int i;  
    register int j;  
    register int k;  
    register int count;  
    register int mergej;/* Sources below this j will be stamped by triples */ 
    register int score;  
    register int temp;   


    mergej = startj -1;  
    for(count=0,j=startj;count<6;count++,j++)    {  
      for(i=starti;i<=stopi;i++) {  
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = NEGI;     
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,0) = (-100);  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,1) = (-100);  
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;     
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,0) = (-100);  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,1) = (-100);  
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = NEGI;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,0) = (-100); 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,1) = (-100); 
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,0) = (-100);   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,1) = (-100);   
          continue;  
          } /* end of Is not in envelope */ 


        /* For state MATCH, pushing when j - offj <= mergej */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;   
        if( j - 1 <= mergej) {  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,0) = i-1; 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,1) = j-1; 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,2) = DIFF_SEQ;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,3) = i;   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,4) = j;   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,5) = MATCH;   
          }  
        else {  
          for(k=0;k<7;k++)   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,DIFF_SEQ,k);    
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,MATCH) + 0;   
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,0) = i-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,1) = j-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,2) = MATCH; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,3) = i; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,4) = j; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,5) = MATCH; 
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,MATCH,k); 
            }  
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;   
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,0) = i-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,1) = j-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,2) = INSERT_SIGNAL; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,3) = i; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,4) = j; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,5) = MATCH; 
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,INSERT_SIGNAL,k); 
            }  
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,DELETE) + 0;  
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,0) = i-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,1) = j-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,2) = DELETE;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,3) = i; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,4) = j; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,5) = MATCH; 
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,DELETE,k);    
            }  
          }  
        /* Add any movement independant score */ 
        score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);     
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = score;  
        /* Finished with state MATCH */ 


        /* For state INSERT_SIGNAL, pushing when j - offj <= mergej */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;   
        if( j - 1 <= mergej) {  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,0) = i-0; 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,1) = j-1; 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,2) = MATCH;   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,3) = i;   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,4) = j;   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,5) = INSERT_SIGNAL;   
          }  
        else {  
          for(k=0;k<7;k++)   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,MATCH,k);   
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;     
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,0) = i-0;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,1) = j-1;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,2) = INSERT_SIGNAL; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,3) = i; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,4) = j; 
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,5) = INSERT_SIGNAL; 
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,INSERT_SIGNAL,k); 
            }  
          }  
        /* Add any movement independant score */ 
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = score;  
        /* Finished with state INSERT_SIGNAL */ 


        /* For state DELETE, pushing when j - offj <= mergej */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;   
        if( j - 0 <= mergej) {  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,0) = i-1;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,1) = j-0;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,2) = MATCH;  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,3) = i;  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,4) = j;  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,5) = DELETE; 
          }  
        else {  
          for(k=0;k<7;k++)   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 0,MATCH,k);  
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;    
        if( temp > score)    {  
          score = temp;  


          if( j - 0 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,0) = i-1;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,1) = j-0;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,2) = DELETE;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,3) = i;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,4) = j;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,5) = DELETE;   
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 0,DELETE,k);   
            }  
          }  
        /* Add any movement independant score */ 
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = score;     
        /* Finished with state DELETE */ 


        /* For state DIFF_SEQ, pushing when j - offj <= mergej */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);   
        if( j - 5 <= mergej) {  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,0) = i-5;  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,1) = j-5;  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,2) = MATCH;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,3) = i;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,4) = j;    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,5) = DIFF_SEQ; 
          }  
        else {  
          for(k=0;k<7;k++)   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 5,j - 5,MATCH,k);    
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;   
        if( temp > score)    {  
          score = temp;  


          if( j - 6 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,0) = i-6;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,1) = j-6;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,2) = MATCH;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,3) = i;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,4) = j;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,5) = DIFF_SEQ;   
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 6,j - 6,MATCH,k);  
            }  
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;     
        if( temp > score)    {  
          score = temp;  


          if( j - 0 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,0) = i-1;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,1) = j-0;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,2) = DIFF_SEQ;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,3) = i;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,4) = j;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,5) = DIFF_SEQ;   
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 0,DIFF_SEQ,k);   
            }  
          }  


        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;     
        if( temp > score)    {  
          score = temp;  


          if( j - 1 <= mergej)   {  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,0) = i-0;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,1) = j-1;    
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,2) = DIFF_SEQ;   
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,3) = i;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,4) = j;  
            EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,5) = DIFF_SEQ;   
            }  
          else   {  
            for(k=0;k<7;k++) 
              EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,DIFF_SEQ,k);   
            }  
          }  
        /* Add any movement independant score */ 
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = score;   
        /* Finished with state DIFF_SEQ */ 
        }  
      }  
    /* Put back j into * stop j so that calling function gets it correct */ 
    if( stopj == NULL)   
      warn("Bad news... NULL stopj pointer in push dc function. This means that calling function does not know how many cells I have done!");    
    else 
      *stopj = j;    


    return;  
}    


/* Function:  follow_on_dc_EventSignalMat(mat,starti,stopi,startj,stopj,dpenv,perc_done)
 *
 * Descrip: No Description
 *
 * Arg:              mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:           starti [UNKN ] Undocumented argument [int]
 * Arg:            stopi [UNKN ] Undocumented argument [int]
 * Arg:           startj [UNKN ] Undocumented argument [int]
 * Arg:            stopj [UNKN ] Undocumented argument [int]
 * Arg:            dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:        perc_done [UNKN ] Undocumented argument [int]
 *
 */
void follow_on_dc_EventSignalMat(EventSignalMat * mat,int starti,int stopi,int startj,int stopj,DPEnvelope * dpenv,int perc_done) 
{
    int i;   
    int j;   
    int k;   
    int score;   
    int temp;    
    int localshadow[7];  
    long int total;  
    long int num;    


    total = (stopi - starti+1) * (stopj - startj+1); 
    num = 0;     


    for(j=startj;j<=stopj;j++)   { /*for each valid j column*/ 
      for(i=starti;i<=stopi;i++) { /*this is strip*/ 
        num++;   
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = NEGI;     
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;     
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = NEGI;    
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;  
          continue;  
          } /* end of Is not in envelope */ 
        if( num % 1000 == 0 )    
          log_full_error(REPORT,0,"[%d%%%% done]After  mid-j %5d Cells done %d%%%%",perc_done,startj,(num*100)/total);   


        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;   
        /* shift first shadow numbers */ 
        for(k=0;k<7;k++) 
          localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,DIFF_SEQ,k);   
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,MATCH) + 0;   
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,MATCH,k);    
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;   
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,INSERT_SIGNAL,k);    
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,DELETE) + 0;  
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 1,DELETE,k);   
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = score; 
        for(k=0;k<7;k++) 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = localshadow[k];  
        /* Now figure out if any specials need this score */ 
        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;   
        /* shift first shadow numbers */ 
        for(k=0;k<7;k++) 
          localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,MATCH,k);  
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;     
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,INSERT_SIGNAL,k);    
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = score; 
        for(k=0;k<7;k++) 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,k) = localshadow[k];  
        /* Now figure out if any specials need this score */ 
        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;   
        /* shift first shadow numbers */ 
        for(k=0;k<7;k++) 
          localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 0,MATCH,k);  
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;    
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 0,DELETE,k);   
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = score;    
        for(k=0;k<7;k++) 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,k) = localshadow[k]; 
        /* Now figure out if any specials need this score */ 
        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);   
        /* shift first shadow numbers */ 
        for(k=0;k<7;k++) 
          localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 5,j - 5,MATCH,k);  
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;   
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 6,j - 6,MATCH,k);    
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;     
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 1,j - 0,DIFF_SEQ,k); 
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;     
        if( temp  > score )  {  
          score = temp;  
          for(k=0;k<7;k++)   
            localshadow[k] = EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i - 0,j - 1,DIFF_SEQ,k); 
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = score;  
        for(k=0;k<7;k++) 
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = localshadow[k];   
        /* Now figure out if any specials need this score */ 
        /* Finished calculating state DIFF_SEQ */ 
        } /* end of this is strip */ 
      } /* end of for each valid j column */ 


/* Function:  run_up_dc_EventSignalMat(mat,starti,stopi,startj,stopj,dpenv,perc_done)
 *
 * Descrip: No Description
 *
 * Arg:              mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:           starti [UNKN ] Undocumented argument [int]
 * Arg:            stopi [UNKN ] Undocumented argument [int]
 * Arg:           startj [UNKN ] Undocumented argument [int]
 * Arg:            stopj [UNKN ] Undocumented argument [int]
 * Arg:            dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 * Arg:        perc_done [UNKN ] Undocumented argument [int]
 *
 */
}    
void run_up_dc_EventSignalMat(EventSignalMat * mat,int starti,int stopi,int startj,int stopj,DPEnvelope * dpenv,int perc_done) 
{
    register int i;  
    register int j;  
    register int score;  
    register int temp;   
    long int total;  
    long int num;    


    total = (stopi - starti+1) * (stopj - startj+1); 
    if( total <= 0 ) 
      total = 1; 
    num = 0;     


    for(j=startj;j<=stopj;j++)   { /*for each valid j column*/ 
      for(i=starti;i<=stopi;i++) { /*this is strip*/ 
        if( j == startj && i == starti)  
          continue;  
        num++;   
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = NEGI;     
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;     
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = NEGI;    
          EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;  
          continue;  
          } /* end of Is not in envelope */ 
        if( num % 1000 == 0 )    
          log_full_error(REPORT,0,"[%d%%%% done]Before mid-j %5d Cells done %d%%%%",perc_done,stopj,(num*100)/total);    


        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;   
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,MATCH) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-1,DELETE) + 0;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = score; 
        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;   
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = score; 
        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;   
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;    
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = score;    
        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_SHADOW_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);   
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;     
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_SHADOW_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = score;  
        /* Finished calculating state DIFF_SEQ */ 
        } /* end of this is strip */ 
      } /* end of for each valid j column */ 


/* Function:  init_dc_EventSignalMat(mat)
 *
 * Descrip: No Description
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 */
}    
void init_dc_EventSignalMat(EventSignalMat * mat) 
{
    register int i;  
    register int j;  
    register int k;  


    for(j=0;j<8;j++) {  
      for(i=(-6);i<mat->query->len;i++)  {  
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = NEGI;   
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;   
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = NEGI;  
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;    
        for(k=0;k<7;k++) {  
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = (-1);    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,k) = (-1);    
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,k) = (-1);   
          EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = (-1); 
          }  
        }  
      }  


    return;  
}    


/* Function:  start_end_find_end_EventSignalMat(mat,endj)
 *
 * Descrip:    First function used to find end of the best path in the special state !end
 *
 *
 * Arg:         mat [UNKN ] Matrix in small mode [EventSignalMat *]
 * Arg:        endj [WRITE] position of end in j (meaningless in i) [int *]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int start_end_find_end_EventSignalMat(EventSignalMat * mat,int * endj) 
{
    register int j;  
    register int max;    
    register int maxj;   


    max = EventSignalMat_DC_SHADOW_SPECIAL(mat,0,mat->target->len-1,END);    
    maxj = mat->target->len-1;   
    for(j= mat->target->len-2 ;j >= 0 ;j--)  {  
      if( EventSignalMat_DC_SHADOW_SPECIAL(mat,0,j,END) > max )  {  
        max = EventSignalMat_DC_SHADOW_SPECIAL(mat,0,j,END); 
        maxj = j;    
        }  
      }  


    if( endj != NULL)    
      *endj = maxj;  


    return max;  
}    


/* Function:  dc_optimised_start_end_calc_EventSignalMat(*mat,dpenv)
 *
 * Descrip:    Calculates special strip, leaving start/end/score points in shadow matrix
 *             Works off specially laid out memory from steve searle
 *
 *
 * Arg:         *mat [UNKN ] Undocumented argument [EventSignalMat]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean dc_optimised_start_end_calc_EventSignalMat(EventSignalMat *mat,DPEnvelope * dpenv) 
{
    int i;   
    int j;   
    int k;   
    int score;   
    int temp;    
    int leni;    
    int lenj;    
    int localshadow[7];  
    long int total;  
    long int num=0;  
    int * score_pointers;    
    int * shadow_pointers;   
    int * localsp;   
    leni = mat->query->len;  
    lenj = mat->target->len; 
    total = leni * lenj; 


    score_pointers = (int *) calloc (6 * (leni + 6) * 4,sizeof(int));    
    shadow_pointers = (int *) calloc (6 * (leni + 6) * 4 * 8,sizeof(int));   


    for(j=0;j<lenj;j++)  { /*for each j strip*/ 
      for(i=0;i<leni;i++)    { /*for each i position in strip*/ 
        num++;   
        if( dpenv != NULL && is_in_DPEnvelope(dpenv,i,j) == FALSE )  { /*Is not in envelope*/ 
          EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,MATCH) = NEGI;     
          EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;     
          EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,DELETE) = NEGI;    
          EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;  
          continue;  
          } /* end of Is not in envelope */ 
        if( num%1000 == 0)   
          log_full_error(REPORT,0,"%6d Cells done [%2d%%%%]",num,num*100/total); 




        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));    
        /* assign local shadown pointer */ 
        localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 1,DIFF_SEQ,0)); 
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-1,MATCH) + 0 +(Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));     
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 1,MATCH,0));  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0 +(Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));     
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 1,INSERT_SIGNAL,0));  
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-1,DELETE) + 0 +(Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));    
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 1,DELETE,0)); 
          }  
        /* From state START to state MATCH */ 
        temp = EventSignalMat_DC_OPT_SHADOW_SPECIAL(mat,i-1,j-1,START) + 0 + (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j));   
        if( temp  > score )  {  
          score = temp;  
          /* This state [START] is a special for MATCH... push top shadow pointers here */ 
          localshadow[0]= i; 
          localshadow[1]= j; 
          localshadow[2]= MATCH; 
          localshadow[3]= (-1);  
          localshadow[4]= (-1);  
          localshadow[5]= (-1);  
          localshadow[6]= score; 
          localsp = localshadow; 
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
        /* Actually, already done inside scores */ 
         EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,MATCH) = score; 
        for(k=0;k<7;k++) 
          EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,MATCH,k) = localsp[k];  
        /* Now figure out if any specials need this score */ 


        /* state MATCH is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > EventSignalMat_DC_OPT_SHADOW_SPECIAL(mat,i,j,END) )   {  
          EventSignalMat_DC_OPT_SHADOW_SPECIAL(mat,i,j,END) = temp;  
          /* Have to push only bottem half of system here */ 
          for(k=0;k<3;k++)   
            EventSignalMat_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,k) = EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,MATCH,k);    
          EventSignalMat_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,6) = EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,MATCH,6);  
          EventSignalMat_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,3) = i;    
          EventSignalMat_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,4) = j;    
          EventSignalMat_DC_OPT_SHADOW_SPECIAL_SP(mat,i,j,END,5) = MATCH;    
          }  




        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,MATCH) + mat->gap + (0);     
        /* assign local shadown pointer */ 
        localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,MATCH,0));    
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext +(0);    
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,INSERT_SIGNAL,0));  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
        /* Actually, already done inside scores */ 
         EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = score; 
        for(k=0;k<7;k++) 
          EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,k) = localsp[k];  
        /* Now figure out if any specials need this score */ 


        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-0,MATCH) + mat->gap + (0);     
        /* assign local shadown pointer */ 
        localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 0,MATCH,0));    
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext +(0);   
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 0,DELETE,0)); 
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
        /* Actually, already done inside scores */ 
         EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,DELETE) = score;    
        for(k=0;k<7;k++) 
          EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,DELETE,k) = localsp[k]; 
        /* Now figure out if any specials need this score */ 


        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext) + (0);     
        /* assign local shadown pointer */ 
        localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 5,j - 5,MATCH,0));    
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open +(0);  
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 6,j - 6,MATCH,0));  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext +(0);    
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 1,j - 0,DIFF_SEQ,0));   
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext +(0);    
        if( temp  > score )  {  
          score = temp;  
          /* assign local shadown pointer */ 
          localsp = &(EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i - 0,j - 1,DIFF_SEQ,0));   
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
        /* Actually, already done inside scores */ 
         EventSignalMat_DC_OPT_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = score;  
        for(k=0;k<7;k++) 
          EventSignalMat_DC_OPT_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,k) = localsp[k];   
        /* Now figure out if any specials need this score */ 


        /* Finished calculating state DIFF_SEQ */ 


        } /* end of for each i position in strip */ 
      } /* end of for each j strip */ 
    free(score_pointers);    
    free(shadow_pointers);   
    return TRUE;     
}    


/* Function:  init_start_end_linear_EventSignalMat(mat)
 *
 * Descrip: No Description
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 */
void init_start_end_linear_EventSignalMat(EventSignalMat * mat) 
{
    register int i;  
    register int j;  
    for(j=0;j<8;j++) {  
      for(i=(-6);i<mat->query->len;i++)  {  
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,MATCH) = NEGI;   
        EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,MATCH,0) = (-1);  
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;   
        EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,INSERT_SIGNAL,0) = (-1);  
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DELETE) = NEGI;  
        EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DELETE,0) = (-1); 
        EventSignalMat_DC_SHADOW_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;    
        EventSignalMat_DC_SHADOW_MATRIX_SP(mat,i,j,DIFF_SEQ,0) = (-1);   
        }  
      }  


    for(j=(-6);j<mat->target->len;j++)   {  
      EventSignalMat_DC_SHADOW_SPECIAL(mat,0,j,START) = 0;   
      EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,j,START,0) = j;  
      EventSignalMat_DC_SHADOW_SPECIAL(mat,0,j,END) = NEGI;  
      EventSignalMat_DC_SHADOW_SPECIAL_SP(mat,0,j,END,0) = (-1); 
      }  


    return;  
}    


/* Function:  convert_PackAln_to_AlnBlock_EventSignalMat(pal)
 *
 * Descrip:    Converts a path alignment to a label alignment
 *             The label alignment is probably much more useful than the path
 *
 *
 * Arg:        pal [UNKN ] Undocumented argument [PackAln *]
 *
 * Return [UNKN ]  Undocumented return value [AlnBlock *]
 *
 */
AlnBlock * convert_PackAln_to_AlnBlock_EventSignalMat(PackAln * pal) 
{
    AlnConvertSet * acs; 
    AlnBlock * alb;  


    acs = AlnConvertSet_EventSignalMat();    
    alb = AlnBlock_from_PackAln(acs,pal);    
    free_AlnConvertSet(acs); 
    return alb;  
}    


 static char * query_label[] = { "SIGNAL_MATCH","SIGNAL_DARK","SIGNAL_NOVO","DIFF_SIGNAL","END" };   
/* Function:  AlnConvertSet_EventSignalMat(void)
 *
 * Descrip: No Description
 *
 *
 * Return [UNKN ]  Undocumented return value [AlnConvertSet *]
 *
 */
 static char * target_label[] = { "SEQUENCE","GAP","DIFF_SEQ","END" };   
AlnConvertSet * AlnConvertSet_EventSignalMat(void) 
{
    AlnConvertUnit * acu;    
    AlnConvertSet  * out;    


    out = AlnConvertSet_alloc_std(); 


    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = DIFF_SEQ;  
    acu->state2 = MATCH;     
    acu->offi = 1;   
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = MATCH; 
    acu->state2 = MATCH;     
    acu->offi = 1;   
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = INSERT_SIGNAL; 
    acu->state2 = MATCH;     
    acu->offi = 1;   
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = DELETE;    
    acu->state2 = MATCH;     
    acu->offi = 1;   
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = START + 4; 
    acu->is_from_special = TRUE; 
    acu->state2 = MATCH;     
    acu->offi = (-1);    
    acu->offj = 1;   
    acu->label1 = query_label[0];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = MATCH; 
    acu->state2 = INSERT_SIGNAL;     
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[1];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = INSERT_SIGNAL; 
    acu->state2 = INSERT_SIGNAL;     
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[1];    
    acu->label2 = target_label[0];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = MATCH; 
    acu->state2 = DELETE;    
    acu->offi = 1;   
    acu->offj = 0;   
    acu->label1 = query_label[2];    
    acu->label2 = target_label[1];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = DELETE;    
    acu->state2 = DELETE;    
    acu->offi = 1;   
    acu->offj = 0;   
    acu->label1 = query_label[2];    
    acu->label2 = target_label[1];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = MATCH; 
    acu->state2 = DIFF_SEQ;  
    acu->offi = 5;   
    acu->offj = 5;   
    acu->label1 = query_label[3];    
    acu->label2 = target_label[2];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = MATCH; 
    acu->state2 = DIFF_SEQ;  
    acu->offi = 6;   
    acu->offj = 6;   
    acu->label1 = query_label[3];    
    acu->label2 = target_label[2];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = DIFF_SEQ;  
    acu->state2 = DIFF_SEQ;  
    acu->offi = 1;   
    acu->offj = 0;   
    acu->label1 = query_label[3];    
    acu->label2 = target_label[2];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = DIFF_SEQ;  
    acu->state2 = DIFF_SEQ;  
    acu->offi = 0;   
    acu->offj = 1;   
    acu->label1 = query_label[3];    
    acu->label2 = target_label[2];   
    acu = AlnConvertUnit_alloc();    
    add_AlnConvertSet(out,acu);  
    acu->state1 = MATCH; 
    acu->state2 = END + 4;   
    acu->offi = (-1);    
    acu->offj = 0;   
    acu->label1 = query_label[4];    
    acu->label2 = target_label[3];   
    return out;  
}    


/* Function:  PackAln_read_Expl_EventSignalMat(mat)
 *
 * Descrip:    Reads off PackAln from explicit matrix structure
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_read_Expl_EventSignalMat(EventSignalMat * mat) 
{
    EventSignalMat_access_func_holder holder;    


    holder.access_main    = EventSignalMat_explicit_access_main; 
    holder.access_special = EventSignalMat_explicit_access_special;  
    return PackAln_read_generic_EventSignalMat(mat,holder);  
}    


/* Function:  EventSignalMat_explicit_access_main(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int EventSignalMat_explicit_access_main(EventSignalMat * mat,int i,int j,int state) 
{
    return EventSignalMat_EXPL_MATRIX(mat,i,j,state);    
}    


/* Function:  EventSignalMat_explicit_access_special(mat,i,j,state)
 *
 * Descrip: No Description
 *
 * Arg:          mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:            i [UNKN ] Undocumented argument [int]
 * Arg:            j [UNKN ] Undocumented argument [int]
 * Arg:        state [UNKN ] Undocumented argument [int]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int EventSignalMat_explicit_access_special(EventSignalMat * mat,int i,int j,int state) 
{
    return EventSignalMat_EXPL_SPECIAL(mat,i,j,state);   
}    


/* Function:  PackAln_read_generic_EventSignalMat(mat,h)
 *
 * Descrip:    Reads off PackAln from explicit matrix structure
 *
 *
 * Arg:        mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:          h [UNKN ] Undocumented argument [EventSignalMat_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [PackAln *]
 *
 */
PackAln * PackAln_read_generic_EventSignalMat(EventSignalMat * mat,EventSignalMat_access_func_holder h) 
{
    register PackAln * out;  
    int i;   
    int j;   
    int state;   
    int cellscore = (-1);    
    boolean isspecial;   
    PackAlnUnit * pau = NULL;    
    PackAlnUnit * prev = NULL;   


    assert(mat);     
    assert(h.access_main);   
    assert(h.access_special);    


    out = PackAln_alloc_std();   
    if( out == NULL )    
      return NULL;   


    out->score =  find_end_EventSignalMat(mat,&i,&j,&state,&isspecial,h);    


    /* Add final end transition (at the moment we have not got the score! */ 
    if( (pau= PackAlnUnit_alloc()) == NULL  || add_PackAln(out,pau) == FALSE )   {  
      warn("Failed the first PackAlnUnit alloc, %d length of Alignment in EventSignalMat_basic_read, returning a mess.(Sorry!)",out->len);   
      return out;    
      }  


    /* Put in positions for end trans. Remember that coordinates in C style */ 
    pau->i = i;  
    pau->j = j;  
    if( isspecial != TRUE)   
      pau->state = state;    
    else pau->state = state + 4;     
    prev=pau;    
    while( state != START || isspecial != TRUE)  { /*while state != START*/ 


      if( isspecial == TRUE )    
        max_calc_special_EventSignalMat(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore,h);   
      else   
        max_calc_EventSignalMat(mat,i,j,state,isspecial,&i,&j,&state,&isspecial,&cellscore,h);   
      if(i == EventSignalMat_READ_OFF_ERROR || j == EventSignalMat_READ_OFF_ERROR || state == EventSignalMat_READ_OFF_ERROR )    {  
        warn("Problem - hit bad read off system, exiting now");  
        break;   
        }  
      if( (pau= PackAlnUnit_alloc()) == NULL  || add_PackAln(out,pau) == FALSE ) {  
        warn("Failed a PackAlnUnit alloc, %d length of Alignment in EventSignalMat_basic_read, returning partial alignment",out->len);   
        break;   
        }  


      /* Put in positions for block. Remember that coordinates in C style */ 
      pau->i = i;    
      pau->j = j;    
      if( isspecial != TRUE)     
        pau->state = state;  
      else pau->state = state + 4;   
      prev->score = cellscore;   
      prev = pau;    
      } /* end of while state != START */ 


    invert_PackAln(out); 
    return out;  
}    


/* Function:  find_end_EventSignalMat(mat,ri,rj,state,isspecial,h)
 *
 * Descrip: No Description
 *
 * Arg:              mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:               ri [UNKN ] Undocumented argument [int *]
 * Arg:               rj [UNKN ] Undocumented argument [int *]
 * Arg:            state [UNKN ] Undocumented argument [int *]
 * Arg:        isspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:                h [UNKN ] Undocumented argument [EventSignalMat_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int find_end_EventSignalMat(EventSignalMat * mat,int * ri,int * rj,int * state,boolean * isspecial,EventSignalMat_access_func_holder h) 
{
    int j;   
    int max; 
    int maxj;    
    int temp;    


    max = (*h.access_special)(mat,0,mat->target->len-1,END); 
    maxj = mat->target->len-1;   
    for(j= mat->target->len-2 ;j >= 0 ;j--)  {  
      if( (temp =(*h.access_special)(mat,0,j,END)) > max )   {  
        max = temp;  
        maxj = j;    
        }  
      }  


    if( ri != NULL)  
       *ri = 0;  
    if( rj != NULL)  
       *rj = maxj;   
    if( state != NULL)   
       *state = END; 
    if( isspecial != NULL)   
       *isspecial = TRUE;    


    return max;  
}    


/* Function:  EventSignalMat_debug_show_matrix(mat,starti,stopi,startj,stopj,ofp)
 *
 * Descrip: No Description
 *
 * Arg:           mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:        starti [UNKN ] Undocumented argument [int]
 * Arg:         stopi [UNKN ] Undocumented argument [int]
 * Arg:        startj [UNKN ] Undocumented argument [int]
 * Arg:         stopj [UNKN ] Undocumented argument [int]
 * Arg:           ofp [UNKN ] Undocumented argument [FILE *]
 *
 */
void EventSignalMat_debug_show_matrix(EventSignalMat * mat,int starti,int stopi,int startj,int stopj,FILE * ofp) 
{
    register int i;  
    register int j;  


    for(i=starti;i<stopi && i < mat->query->len;i++) {  
      for(j=startj;j<stopj && j < mat->target->len;j++)  {  
        fprintf(ofp,"Cell [%d - %d]\n",i,j);     
        fprintf(ofp,"State MATCH %d\n",EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH));   
        fprintf(ofp,"State INSERT_SIGNAL %d\n",EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL));   
        fprintf(ofp,"State DELETE %d\n",EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE)); 
        fprintf(ofp,"State DIFF_SEQ %d\n",EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ)); 
        fprintf(ofp,"\n\n"); 
        }  
      }  


}    


/* Function:  max_calc_EventSignalMat(mat,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore,h)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 * Arg:                 h [UNKN ] Undocumented argument [EventSignalMat_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_calc_EventSignalMat(EventSignalMat * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore,EventSignalMat_access_func_holder h) 
{
    register int temp;   
    register int cscore; 


    *reti = (*retj) = (*retstate) = EventSignalMat_READ_OFF_ERROR;   


    if( i < 0 || j < 0 || i > mat->query->len || j > mat->target->len)   {  
      warn("In EventSignalMat matrix special read off - out of bounds on matrix [i,j is %d,%d state %d in standard matrix]",i,j,state);  
      return -1;     
      }  


    /* Then you have to select the correct switch statement to figure out the readoff      */ 
    /* Somewhat odd - reverse the order of calculation and return as soon as it is correct */ 
    cscore = (*h.access_main)(mat,i,j,state);    
    switch(state)    { /*Switch state */ 
      case MATCH :   
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == (*h.access_special)(mat,i - 1,j - 1,START) ) {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = START; 
          *retspecial = TRUE;    
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_special)(mat,i-1,j-1,START);    
            }  
          return (*h.access_main)(mat,i - 1,j - 1,START);    
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == (*h.access_main)(mat,i - 1,j - 1,DELETE) )   {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = DELETE;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-1,DELETE);  
            }  
          return (*h.access_main)(mat,i - 1,j - 1,DELETE);   
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == (*h.access_main)(mat,i - 1,j - 1,INSERT_SIGNAL) )    {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = INSERT_SIGNAL; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-1,INSERT_SIGNAL);   
            }  
          return (*h.access_main)(mat,i - 1,j - 1,INSERT_SIGNAL);    
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == (*h.access_main)(mat,i - 1,j - 1,MATCH) )    {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-1,MATCH);   
            }  
          return (*h.access_main)(mat,i - 1,j - 1,MATCH);    
          }  
        temp = cscore - (0) -  (Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j)); 
        if( temp == (*h.access_main)(mat,i - 1,j - 1,DIFF_SEQ) ) {  
          *reti = i - 1; 
          *retj = j - 1; 
          *retstate = DIFF_SEQ;  
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-1,DIFF_SEQ);    
            }  
          return (*h.access_main)(mat,i - 1,j - 1,DIFF_SEQ);     
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      case INSERT_SIGNAL :   
        temp = cscore - (mat->gapext) -  (0);    
        if( temp == (*h.access_main)(mat,i - 0,j - 1,INSERT_SIGNAL) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = INSERT_SIGNAL; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,INSERT_SIGNAL);   
            }  
          return (*h.access_main)(mat,i - 0,j - 1,INSERT_SIGNAL);    
          }  
        temp = cscore - (mat->gap) -  (0);   
        if( temp == (*h.access_main)(mat,i - 0,j - 1,MATCH) )    {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,MATCH);   
            }  
          return (*h.access_main)(mat,i - 0,j - 1,MATCH);    
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      case DELETE :  
        temp = cscore - (mat->gapext) -  (0);    
        if( temp == (*h.access_main)(mat,i - 1,j - 0,DELETE) )   {  
          *reti = i - 1; 
          *retj = j - 0; 
          *retstate = DELETE;    
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-0,DELETE);  
            }  
          return (*h.access_main)(mat,i - 1,j - 0,DELETE);   
          }  
        temp = cscore - (mat->gap) -  (0);   
        if( temp == (*h.access_main)(mat,i - 1,j - 0,MATCH) )    {  
          *reti = i - 1; 
          *retj = j - 0; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-0,MATCH);   
            }  
          return (*h.access_main)(mat,i - 1,j - 0,MATCH);    
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      case DIFF_SEQ :    
        temp = cscore - (mat->seqdiff_ext) -  (0);   
        if( temp == (*h.access_main)(mat,i - 0,j - 1,DIFF_SEQ) ) {  
          *reti = i - 0; 
          *retj = j - 1; 
          *retstate = DIFF_SEQ;  
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-0,j-1,DIFF_SEQ);    
            }  
          return (*h.access_main)(mat,i - 0,j - 1,DIFF_SEQ);     
          }  
        temp = cscore - (mat->seqdiff_ext) -  (0);   
        if( temp == (*h.access_main)(mat,i - 1,j - 0,DIFF_SEQ) ) {  
          *reti = i - 1; 
          *retj = j - 0; 
          *retstate = DIFF_SEQ;  
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-1,j-0,DIFF_SEQ);    
            }  
          return (*h.access_main)(mat,i - 1,j - 0,DIFF_SEQ);     
          }  
        temp = cscore - (mat->seqdiff_open) -  (0);  
        if( temp == (*h.access_main)(mat,i - 6,j - 6,MATCH) )    {  
          *reti = i - 6; 
          *retj = j - 6; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-6,j-6,MATCH);   
            }  
          return (*h.access_main)(mat,i - 6,j - 6,MATCH);    
          }  
        temp = cscore - ((mat->seqdiff_open+mat->seqdiff_ext)) -  (0);   
        if( temp == (*h.access_main)(mat,i - 5,j - 5,MATCH) )    {  
          *reti = i - 5; 
          *retj = j - 5; 
          *retstate = MATCH; 
          *retspecial = FALSE;   
          if( cellscore != NULL) {  
            *cellscore = cscore - (*h.access_main)(mat,i-5,j-5,MATCH);   
            }  
          return (*h.access_main)(mat,i - 5,j - 5,MATCH);    
          }  
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      default:   
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found!",i,j,state);  
        return (-1); 
      } /* end of Switch state  */ 
}    


/* Function:  max_calc_special_EventSignalMat(mat,i,j,state,isspecial,reti,retj,retstate,retspecial,cellscore,h)
 *
 * Descrip: No Description
 *
 * Arg:               mat [UNKN ] Undocumented argument [EventSignalMat *]
 * Arg:                 i [UNKN ] Undocumented argument [int]
 * Arg:                 j [UNKN ] Undocumented argument [int]
 * Arg:             state [UNKN ] Undocumented argument [int]
 * Arg:         isspecial [UNKN ] Undocumented argument [boolean]
 * Arg:              reti [UNKN ] Undocumented argument [int *]
 * Arg:              retj [UNKN ] Undocumented argument [int *]
 * Arg:          retstate [UNKN ] Undocumented argument [int *]
 * Arg:        retspecial [UNKN ] Undocumented argument [boolean *]
 * Arg:         cellscore [UNKN ] Undocumented argument [int *]
 * Arg:                 h [UNKN ] Undocumented argument [EventSignalMat_access_func_holder]
 *
 * Return [UNKN ]  Undocumented return value [int]
 *
 */
int max_calc_special_EventSignalMat(EventSignalMat * mat,int i,int j,int state,boolean isspecial,int * reti,int * retj,int * retstate,boolean * retspecial,int * cellscore,EventSignalMat_access_func_holder h) 
{
    register int temp;   
    register int cscore; 


    *reti = (*retj) = (*retstate) = EventSignalMat_READ_OFF_ERROR;   


    if( j < 0 || j > mat->target->len)   {  
      warn("In EventSignalMat matrix special read off - out of bounds on matrix [j is %d in special]",j);    
      return -1;     
      }  


    cscore = (*h.access_special)(mat,i,j,state); 
    switch(state)    { /*switch on special states*/ 
      case START :   
      case END :     
        /* source MATCH is from main matrix */ 
        for(i= mat->query->len-1;i >= 0 ;i--)    { /*for i >= 0*/ 
          temp = cscore - (0) - (0);     
          if( temp == (*h.access_main)(mat,i - 0,j - 0,MATCH) )  {  
            *reti = i - 0;   
            *retj = j - 0;   
            *retstate = MATCH;   
            *retspecial = FALSE; 
            if( cellscore != NULL)   {  
              *cellscore = cscore - (*h.access_main)(mat,i-0,j-0,MATCH);     
              }  
            return (*h.access_main)(mat,i - 0,j - 0,MATCH) ;     
            }  
          } /* end of for i >= 0 */ 
      default:   
        warn("Major problem (!) - in EventSignalMat read off, position %d,%d state %d no source found  dropped into default on source switch!",i,j,state);   
        return (-1); 
      } /* end of switch on special states */ 
}    


/* Function:  calculate_EventSignalMat(mat)
 *
 * Descrip:    This function calculates the EventSignalMat matrix when in explicit mode
 *             To allocate the matrix use /allocate_Expl_EventSignalMat
 *
 *
 * Arg:        mat [UNKN ] EventSignalMat which contains explicit basematrix memory [EventSignalMat *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean calculate_EventSignalMat(EventSignalMat * mat) 
{
    int i;   
    int j;   
    int leni;    
    int lenj;    
    int tot; 
    int num; 


    if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT )  {  
      warn("in calculate_EventSignalMat, passed a non Explicit matrix type, cannot calculate!"); 
      return FALSE;  
      }  


    leni = mat->leni;    
    lenj = mat->lenj;    
    tot = leni * lenj;   
    num = 0; 


    start_reporting("EventSignalMat Matrix calculation: ");  
    for(j=0;j<lenj;j++)  {  
      auto int score;    
      auto int temp;     
      for(i=0;i<leni;i++)    {  
        if( num%1000 == 0 )  
          log_full_error(REPORT,0,"[%7d] Cells %2d%%%%",num,num*100/tot);    
        num++;   


        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;    
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,MATCH) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,DELETE) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state START to state MATCH */ 
        temp = EventSignalMat_EXPL_SPECIAL(mat,i-1,j-1,START) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH) = score;  


        /* state MATCH is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > EventSignalMat_EXPL_SPECIAL(mat,i,j,END) )    {  
          EventSignalMat_EXPL_SPECIAL(mat,i,j,END) = temp;   
          }  




        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;    
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL) = score;  


        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;    
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE) = score; 


        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);    
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ) = score;   


        /* Finished calculating state DIFF_SEQ */ 
        }  


      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      }  
    stop_reporting();    
    return TRUE;     
}    


/* Function:  calculate_dpenv_EventSignalMat(mat,dpenv)
 *
 * Descrip:    This function calculates the EventSignalMat matrix when in explicit mode, subject to the envelope
 *
 *
 * Arg:          mat [UNKN ] EventSignalMat which contains explicit basematrix memory [EventSignalMat *]
 * Arg:        dpenv [UNKN ] Undocumented argument [DPEnvelope *]
 *
 * Return [UNKN ]  Undocumented return value [boolean]
 *
 */
boolean calculate_dpenv_EventSignalMat(EventSignalMat * mat,DPEnvelope * dpenv) 
{
    int i;   
    int j;   
    int k;   
    int starti;  
    int startj;  
    int endi;    
    int endj;    
    int tot; 
    int num; 
    int should_calc; 


    if( mat->basematrix->type != BASEMATRIX_TYPE_EXPLICIT )  {  
      warn("in calculate_EventSignalMat, passed a non Explicit matrix type, cannot calculate!"); 
      return FALSE;  
      }  


    prepare_DPEnvelope(dpenv);   
    starti = dpenv->starti;  
    if( starti < 0 ) 
      starti = 0;    
    startj = dpenv->startj;  
    if( startj < 0 ) 
      startj = 0;    
    endi = dpenv->endi;  
    if( endi > mat->leni )   
      endi = mat->leni;  
    endj = dpenv->endj;  
    if( endj > mat->lenj )   
      endj = mat->lenj;  
    tot = (endi-starti) * (endj-startj); 
    num = 0; 


    for(j=startj-6;j<endj;j++)   {  
      for(i=6;i<mat->leni;i++)   {  
        EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH) = NEGI;    
        EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;    
        EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE) = NEGI;   
        EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ) = NEGI; 
        }  
      }  
    for(j=-6;j<mat->lenj;j++)    {  
      EventSignalMat_EXPL_SPECIAL(mat,i,j,START) = 0;    
      EventSignalMat_EXPL_SPECIAL(mat,i,j,END) = NEGI;   
      }  


    start_reporting("EventSignalMat Matrix calculation: ");  
    for(j=startj;j<endj;j++) {  
      auto int score;    
      auto int temp;     
      for(i=starti;i<endi;i++)   {  
        /* Check if is in envelope - code identical to is_in_DPEnvelope, but aggressively inlined here for speed */ 
        should_calc = 0; 
        for(k=0;k<dpenv->len;k++)    {  
          auto DPUnit * u;   
          u = dpenv->dpu[k]; 
          switch(u->type)    {  
            case DPENV_RECT :    
              if( i >= u->starti && j >= u->startj && i <= (u->starti+u->height) && j <= (u->startj+u->length))  
                should_calc = 1;     
              break; 
            case DPENV_DIAG :    
              if(  abs( (i-j) - (u->starti-u->startj)) <= u->height && i+j >= u->starti+u->startj && i+j+u->length >= u->starti+u->startj)   
                should_calc = 1;     
              break; 
            }  
          if( should_calc == 1 ) 
            break;   
          }  
        if( should_calc == 0)    {  
          EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH) = NEGI;  
          EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL) = NEGI;  
          EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE) = NEGI; 
          EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ) = NEGI;   
          continue;  
          }  


        if( num%1000 == 0 )  
          log_full_error(REPORT,0,"[%7d] Cells %2d%%%%",num,num*100/tot);    
        num++;   


        /* For state MATCH */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,DIFF_SEQ) + 0;    
        /* From state MATCH to state MATCH */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,MATCH) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state INSERT_SIGNAL to state MATCH */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,INSERT_SIGNAL) + 0;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DELETE to state MATCH */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-1,DELETE) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state START to state MATCH */ 
        temp = EventSignalMat_EXPL_SPECIAL(mat,i-1,j-1,START) + 0;   
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for MATCH */ 
        /* Add any movement independant score and put away */ 
         score += Score_offset_SignalMap(mat->sm,mat->query,i,mat->target,j);    
         EventSignalMat_EXPL_MATRIX(mat,i,j,MATCH) = score;  


        /* state MATCH is a source for special END */ 
        temp = score + (0) + (0) ;   
        if( temp > EventSignalMat_EXPL_SPECIAL(mat,i,j,END) )    {  
          EventSignalMat_EXPL_SPECIAL(mat,i,j,END) = temp;   
          }  




        /* Finished calculating state MATCH */ 


        /* For state INSERT_SIGNAL */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-0,j-1,MATCH) + mat->gap;    
        /* From state INSERT_SIGNAL to state INSERT_SIGNAL */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-0,j-1,INSERT_SIGNAL) + mat->gapext;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for INSERT_SIGNAL */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_EXPL_MATRIX(mat,i,j,INSERT_SIGNAL) = score;  


        /* Finished calculating state INSERT_SIGNAL */ 


        /* For state DELETE */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-1,j-0,MATCH) + mat->gap;    
        /* From state DELETE to state DELETE */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-0,DELETE) + mat->gapext;     
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DELETE */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_EXPL_MATRIX(mat,i,j,DELETE) = score; 


        /* Finished calculating state DELETE */ 


        /* For state DIFF_SEQ */ 
        /* setting first movement to score */ 
        score = EventSignalMat_EXPL_MATRIX(mat,i-5,j-5,MATCH) + (mat->seqdiff_open+mat->seqdiff_ext);    
        /* From state MATCH to state DIFF_SEQ */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-6,j-6,MATCH) + mat->seqdiff_open;    
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-1,j-0,DIFF_SEQ) + mat->seqdiff_ext;  
        if( temp  > score )  {  
          score = temp;  
          }  
        /* From state DIFF_SEQ to state DIFF_SEQ */ 
        temp = EventSignalMat_EXPL_MATRIX(mat,i-0,j-1,DIFF_SEQ) + mat->seqdiff_ext;  
        if( temp  > score )  {  
          score = temp;  
          }  


        /* Ok - finished max calculation for DIFF_SEQ */ 
        /* Add any movement independant score and put away */ 
         EventSignalMat_EXPL_MATRIX(mat,i,j,DIFF_SEQ) = score;   


        /* Finished calculating state DIFF_SEQ */ 
        }  


      /* Special state START has no special to special movements */ 


      /* Special state END has no special to special movements */ 
      }  
    stop_reporting();    
    return TRUE;     
}    


/* Function:  EventSignalMat_alloc(void)
 *
 * Descrip:    Allocates structure: assigns defaults if given 
 *
 *
 *
 * Return [UNKN ]  Undocumented return value [EventSignalMat *]
 *
 */
EventSignalMat * EventSignalMat_alloc(void) 
{
    EventSignalMat * out;   /* out is exported at end of function */ 


    /* call ckalloc and see if NULL */ 
    if((out=(EventSignalMat *) ckalloc (sizeof(EventSignalMat))) == NULL)    {  
      warn("EventSignalMat_alloc failed ");  
      return NULL;  /* calling function should respond! */ 
      }  
    out->dynamite_hard_link = 1; 
#ifdef PTHREAD   
    pthread_mutex_init(&(out->dynamite_mutex),NULL);     
#endif   
    out->basematrix = NULL;  
    out->shatter = NULL; 
    out->leni = 0;   
    out->lenj = 0;   


    return out;  
}    


/* Function:  free_EventSignalMat(obj)
 *
 * Descrip:    Free Function: removes the memory held by obj
 *             Will chain up to owned members and clear all lists
 *
 *
 * Arg:        obj [UNKN ] Object that is free'd [EventSignalMat *]
 *
 * Return [UNKN ]  Undocumented return value [EventSignalMat *]
 *
 */
EventSignalMat * free_EventSignalMat(EventSignalMat * obj) 
{
    int return_early = 0;    


    if( obj == NULL) {  
      warn("Attempting to free a NULL pointer to a EventSignalMat obj. Should be trappable");    
      return NULL;   
      }  


#ifdef PTHREAD   
    assert(pthread_mutex_lock(&(obj->dynamite_mutex)) == 0); 
#endif   
    if( obj->dynamite_hard_link > 1)     {  
      return_early = 1;  
      obj->dynamite_hard_link--; 
      }  
#ifdef PTHREAD   
    assert(pthread_mutex_unlock(&(obj->dynamite_mutex)) == 0);   
#endif   
    if( return_early == 1)   
      return NULL;   
    if( obj->basematrix != NULL) 
      free_BaseMatrix(obj->basematrix);  
    if( obj->shatter != NULL)    
      free_ShatterMatrix(obj->shatter);  
    /* obj->query is linked in */ 
    /* obj->target is linked in */ 
    /* obj->sm is linked in */ 
    /* obj->gap is linked in */ 
    /* obj->gapext is linked in */ 
    /* obj->seqdiff_open is linked in */ 
    /* obj->seqdiff_ext is linked in */ 


    ckfree(obj); 
    return NULL; 
}    





#ifdef _cplusplus
}
#endif
