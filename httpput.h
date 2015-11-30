/* 
 * File:   httpput.h
 * Author: bart
 *
 * Created on 8 april 2015, 19:35
 */

#ifndef HTTPPUT_H
#define	HTTPPUT_H

#ifdef	__cplusplus
extern "C" {
#endif

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream);

int httpput(char *file, char *url);


#ifdef	__cplusplus
}
#endif

#endif	/* HTTPPUT_H */

