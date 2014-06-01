#ifndef APP_H_
#define APP_H_

#define STACK_SIZE 256


// typedefs
typedef struct GlobalDataStr_t {
	uint8_t guard;
	uint8_t inc;
	uint8_t t1;
	uint8_t t2;
}GlobalDataStr_t, *PGlobalDataStr_t;



extern KOS_STK thread1Stack[];

extern KOS_STK thread2Stack[];

extern KOS_STK thread3Stack[];

extern GlobalDataStr_t shared;

/** 
 * Thread 1.
 * 
 * Casts pData to GlobalDataStr_t and increments the variable inc
 * 
 * @param pData is a pointer to shared data
 * @return error code
 * 
 */
extern
void thread1Entry(void *pData);


/** 
 * Thread 2.
 * 
 * Casts pData to GlobalDataStr_t and decrements the variable inc
 * 
 * @param pData is a pointer to shared data
 * @return error code
 * 
 */
extern
void thread2Entry(void *pData);


/** 
 * Thread 3.
 * 
 * Casts pData to GlobalDataStr_t and checks the inc variable.
 * Returns error if inc is larger then 3.
 * 
 * @param pData is a pointer to shared data
 * @return error code
 * 
 */
extern
void thread3Entry(void *pData);


#endif /*APP_H_*/
