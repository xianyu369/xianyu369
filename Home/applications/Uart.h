#ifndef APPLICATIONS_UART_H_
#define APPLICATIONS_UART_H_

void Uart_thread_start(void);
void Uart_Send_thread_start(void);
void Uart_Send_thread_entry();
void Uart_thread_entry();
void Uart_Init(void);

#endif
