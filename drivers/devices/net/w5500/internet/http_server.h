#ifndef  _HTTP_SERVER_H_
#define  _HTTP_SERVER_H_

#include "types.h"

#define HTTP_SERVER_PORT		80		/**< Http server well-known port number */

/* HTTP Method */
#define		METHOD_ERR			0				/**< Error Method. */
#define		METHOD_GET			1				/**< GET Method.   */
#define		METHOD_HEAD			2				/**< HEAD Method.  */
#define		METHOD_POST			3				/**< POST Method.  */

/* HTTP GET Method */
#define		PTYPE_ERR			0				/**< Error file. */
#define		PTYPE_HTML			1				/**< HTML	file.  */
#define		PTYPE_GIF			2				/**< GIF	file.  */
#define		PTYPE_TEXT			3				/**< TEXT file.  */
#define		PTYPE_JPEG			4				/**< JPEG file.  */
#define		PTYPE_FLASH			5				/**< FLASH file. */
#define		PTYPE_MPEG			6				/**< MPEG file.  */
#define		PTYPE_PDF			7				/**< PDF file.   */
#define 	PTYPE_CGI			8				/**< CGI */
#define 	PTYPE_PL			9 		        /**pl*/



/* HTML Doc. for ERROR */
#define ERROR_HTML_PAGE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nSorry, the page you requested was not found.\r\n</BODY>\r\n</HTML>\r\n\0"
//static char  ERROR_HTML_PAGE[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 78\r\n\r\n<HTML>\r\n<BODY>\r\nSorry, the page you requested was not found.\r\n</BODY>\r\n</HTML>\r\n\0";

#define ERROR_REQUEST_PAGE "HTTP/1.1 400 OK\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<HTML>\r\n<BODY>\r\nInvalid request.\r\n</BODY>\r\n</HTML>\r\n\0"
//static char ERROR_REQUEST_PAGE[] = "HTTP/1.1 400 OK\r\nContent-Type: text/html\r\nContent-Length: 50\r\n\r\n<HTML>\r\n<BODY>\r\nInvalid request.\r\n</BODY>\r\n</HTML>\r\n\0";

#define RETURN_CGI_PAGE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 59\r\n\r\n<html><head><title>iWeb - Configuration</title></head><BODY>CGI command was executed.</BODY></HTML>\0"


/* Response header for HTML*/
#define RES_HTMLHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
//static PROGMEM char RES_HTMLHEAD_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: ";
/* Response head for TEXT */
#define RES_TEXTHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: "

/* Response head for GIF */
#define RES_GIFHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: image/gif\r\nContent-Length: "

/* Response head for JPEG */
#define RES_JPEGHEAD_OK	"HTTP/1.1 200 OK\r\nContent-Type: image/jpeg\r\nContent-Length: "		

/* Response head for FLASH */
#define RES_FLASHHEAD_OK "HTTP/1.1 200 OK\r\nContent-Type: application/x-shockwave-flash\r\nContent-Length: "
//static PROGMEM char RES_FLASHHEAD_OK[] = "HTTP/1.1 200 OK\r\nContent-Type: application/x-shockwave-flash\r\nContent-Length: ";

/* Response head for MPEG */
#define RES_MPEGHEAD_OK "HTTP/1.1 200 OK\r\nContent-Type: video/mpeg\r\nContent-Length: "	

/* Response head for PDF */
#define RES_PDFHEAD_OK "HTTP/1.1 200 OK\r\nContent-Type: application/pdf\r\nContent-Length: "

//digital I/O out put control result response
#define DOUT_RES_1  "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\n\r\n1"
#define DOUT_RES_0  "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 1\r\n\r\n0"

#define MAX_URI_SIZE	1460//(TX_RX_MAX_BUF_SIZE/2 - sizeof(char)*2)		

/**
 @brief 	Structure of HTTP REQUEST 
 */
typedef struct _st_http_request
{
  uint8	METHOD;															/**< request method(METHOD_GET...). */
  uint8	TYPE;															/**< request type(PTYPE_HTML...).   */
  int8	URI[MAX_URI_SIZE];												/**< request file name.             */
}st_http_request;

void unescape_http_url(char * url);										/* convert escape character to ascii */

void parse_http_request(st_http_request *, uint8 *);					/* parse request from peer */
	
void find_http_uri_type(uint8 *, char *);								/* find MIME type of a file */

void make_http_response_head(unsigned char *, char, u_long);			/* make response header */

unsigned char* get_http_param_value(char* uri, char* param_name);	/* get the user-specific parameter value */

#endif	/* end of __HTTPD_H__ */ 
