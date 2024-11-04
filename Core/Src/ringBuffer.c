/**
  ******************************************************************************
  * @file    ringBuffer.h
  * @brief   Communication received ring-buffer
  ******************************************************************************
*/

#include "ringBuffer.h"

#define TRUE        1
#define FALSE       0


t_rb OledRecvBuf, WifiRecvBuf, bcRecvBuf, BarcodeRecvBuf;

//rb_put함수는 링버퍼에 데이터를 저장하는 함수이다.
uint8_t rb_put( t_rb *rb , uint8_t d )
{
    //uint16_t nhead = (rb->head+1) % (SERIAL_BUF_SIZE);
    uint32_t nhead = (rb->head + 1) & (SERIAL_BUF_SIZE - 1);    // buffer size는 2n으로 
    if(rb->tail == nhead){
        return FALSE;
    }
    rb->data[rb->head] = d;
    rb->head = nhead;
    return TRUE;
}
//rb_get함수는 링퍼버의 데이터를 읽어오는 함수이다.
uint8_t rb_get( t_rb *rb, uint8_t *err) // pop -> send Ethernet
{
    uint8_t d;
    uint32_t ntail = (rb->tail + 1) & (SERIAL_BUF_SIZE - 1);
    *err = FALSE;
    if(rb->head == rb->tail){
        *err = TRUE;
        return 0;       // buffer non-write -> empty
    }
    d = rb->data[rb->tail];
    rb->tail = ntail;
    return d;       // buffer read
}
//finder함수는 링버퍼의 데이터에서 특정 값을 찾아내는 함수이다.
uint8_t finder ( t_rb *rb, uint8_t ch ) // new line finder
{
    uint16_t i;
    uint16_t cnt;
    if(rb->head == rb->tail){
        return 0;
    }
    else if(rb->head > rb->tail){
        for(i=rb->tail; i < rb->head; i++){
            if(rb->data[i] == ch){
                return (i+1);
            }
        }
    }
    else{
        for(i=rb->tail; i  < SERIAL_BUF_SIZE; i++){
            if(rb->data[i] == ch){
                return (i+1);
            }
        }
        cnt = i;
        for(i=0; i<rb->head; i++){
            if(rb->data[i] == ch){
                return (cnt+i+1);
            }
        }
    }
    return 0;
}

uint16_t rb_removeAt( uint8_t* buf, uint16_t count)
{
    // 첫번째 인덱스 삭제
    memcpy(&buf[0], &buf[1], sizeof(uint8_t) * (count - 1));
    //for (int i = (index + 1); i < (count - index - 1); ++i)
    //    buf[i - 1] = buf[i];
    // 빈자리 0으로 채우기
    buf[count - 1] = 0;

    return count - 1;
}
uint16_t rb_removeRange( uint8_t* buf, uint16_t count, uint16_t length)
{
    // 뒷공간 내용 앞으로 복사
    memcpy(&buf[0], &buf[length], sizeof(uint8_t) * (count - length)); 

    // 빈자리 0으로 채우기
    for (int i = (count - length); i < count; ++i)
        buf[i] = 0;

    return count - length;
}