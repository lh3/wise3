#ifdef _cplusplus
extern "C" {
#endif
#include "simplesocketclient.h"


# line 31 "simplesocketclient.dy"
char * read_buffer_simple_socket_impl(void * handle,char * buffer,int maxsize)
{
  int ret;
  Wise2_SimpleRead_Socket * s = (Wise2_SimpleRead_Socket *) handle;
  char * r;
  
  if( s->has_ended == TRUE ) {
    return NULL;
  }

  return fgets(buffer,maxsize,s->socket_ifp);
}

# line 44 "simplesocketclient.dy"
boolean is_end_simple_socket_impl(void * handle)
{
  Wise2_SimpleRead_Socket * s = (Wise2_SimpleRead_Socket *) handle;

  if( s->has_ended == TRUE ) {
    return TRUE;
  } else {
    return FALSE;
  }

}


# line 57 "simplesocketclient.dy"
void close_and_free_simple_socket_impl(void * handle)
{
  Wise2_SimpleRead_Socket * s = (Wise2_SimpleRead_Socket *) handle;
  close( s->socket );
  free(s);
}


# line 65 "simplesocketclient.dy"
Wise2ReadStreamInterface * open_simple_socket_for_reading(char * hostname,int port_number)
{
  Wise2ReadStreamInterface * out;
  Wise2_SimpleRead_Socket * handle;
  
  struct sockaddr_in server;
  struct hostent * hp;

  out = malloc(sizeof(Wise2ReadStreamInterface));
  out->read_buffer = read_buffer_simple_socket_impl;
  out->is_end = is_end_simple_socket_impl;
  out->close_and_free_handle = close_and_free_simple_socket_impl;


  handle = malloc(sizeof(Wise2_SimpleRead_Socket));
  handle->has_ended = 0;
  handle->socket = socket(AF_INET, SOCK_STREAM, 0);

  server.sin_family = AF_INET;
  hp = gethostbyname(hostname);
  bcopy( hp->h_addr, &server.sin_addr, hp->h_length);
  server.sin_port = htons(port_number);

  connect(handle->socket, &server, sizeof(server));

  /*  fcntl(handle->socket,F_SETFL,*/

  write(handle->socket,"<WISE2-START-SIMPLE-STREAM>\n",strlen("<WISE2-START-SIMPLE-STREAM>\n"));
  
  fprintf(stderr,"written start...\n");

  handle->socket_ifp = fdopen(handle->socket,"r");

  out->handle = (void *) handle;
 
  return out;
}

# line 82 "simplesocketclient.c"

#ifdef _cplusplus
}
#endif
