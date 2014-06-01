#ifndef OS_CORE_H_
#define OS_CORE_H_



#define KOS_STK uint32_t

#define KOS_MAX_PRIORITIES      255
#define KOS_LOWEST_PRIORITY     KOS_MAX_PRIORITIES-1

//--------------------------------------------------------------
// typedefs

typedef void (threadfunc_t)(void *pData);



/** 
 * Initialize the OS.
 * 
 * Sets the first thread to the idle thread.
 * Must call before kos_StartOS()
 * 
 * @return error code
 */
extern
uint32_t kos_InitOS(void);


/** 
 * Adds a thread function to the schedular.
 * 
 * The lowest pri is 0, the highest 255. The name can be 
 * KOS_MAX_THREAD_NAME_LEN characters in length. The function pointer
 * must be type pthreadfunc_t.
 * 
 * example: kos_CreateThread(  50, "Thread 1", thread1Entry);
 * 
 * @param pri is the priority of the thread
 * @param pszName is the name of the thread
 * @param pFunc the thread function
 * @return error code
 */
extern
uint32_t kos_CreateThread( uint8_t pri, const char* pszName, KOS_STK *stack, uint32_t stk_size, threadfunc_t *pThreadFunc, void *pVoid);


/** 
 * Start the OS
 * 
 * Starts the schedular running.
 * 
 * @return error code
 */
extern
uint32_t kos_StartOS(void);



#endif /*OS_CORE_H_*/
