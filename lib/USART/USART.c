#include "usart.h"
# define TX1 9
# define RX1 10

# define TX2 2
# define RX2 3

# define TX3 10
# define RX3 11
void usart_init(USART_TypeDef* USARTx, int baudrate) {
    if (USARTx == USART1){
        RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN;
        RCC -> APB2ENR |= RCC_APB2ENR_USART1EN; //habilita USART1
        GPIOA -> CRH &=~ (0xF<<((TX1%8)*4));
        GPIOA -> CRH &=~ (0xF<<((RX1%8)*4));
        GPIOA -> CRH |= (0x9<<((TX1%8)*4)); //0x9 = alternative output pushpull
        GPIOA -> CRH |= (0x4<<((RX1%8)*4)); //0x4 = floating input
        USARTx->BRR = (16*baudrate)/72000000;//velocidad de comunicacion
    }
    else if (USARTx == USART2){
        RCC ->APB2ENR |= RCC_APB2ENR_IOPAEN;
        RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
        GPIOA -> CRL &=~ (0xF<<((TX2)*4));
        GPIOA -> CRL &=~ (0xF<<((RX2)*4));
        GPIOA -> CRL |= (0x9<<((TX2)*4));
        GPIOA -> CRL |= (0x4<<((RX2)*4));
        USARTx->BRR = (16*baudrate)/36000000;
    }
    else if (USARTx == USART3){
        RCC -> APB2ENR |= RCC_APB2ENR_IOPBEN;
        RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
        GPIOB -> CRH &=~ (0xF<<((TX3%8)*4));
        GPIOB -> CRH &=~ (0xF<<((RX3%8)*4));
        GPIOB -> CRH |= (0x9<<((TX3%8)*4));
        GPIOB -> CRH |= (0x4<<((RX3%8)*4));
        USARTx->BRR = (16*baudrate)/36000000;
    }
    //registro de control principal CR1 
    USARTx->CR1|= USART_CR1_UE;//Habilita el USART
    USARTx->CR1|= USART_CR1_TE;//Habilita transmisor
    USARTx->CR1|= USART_CR1_RE;//Habilita receptor
}
void usart_send_char(USART_TypeDef* USARTx, char c){
    while (!(USARTx->SR & USART_SR_TXE));// Espera a que haya datos transmitidos y que DR este libre
    USARTx->DR = c;//envia el caracter
}
void usart_send_string(USART_TypeDef* USARTx, char* str){
    while (*str){
        usart_send_char(USARTx, *str++);//enviamos el caracter ingresado a la funcion send_char
    }
}
char usart_recibir_char(USART_TypeDef* USARTx){
    while (!(USARTx->SR & USART_SR_RXNE));// Espera a que haya datos recibidos
    return USARTx->DR;// Devuelve el carácter recibido
}