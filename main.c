/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
uint8 result,i,RA,status,b[]={0x01,0x10},p;
volatile uint16 l,h,dato;
volatile uint8 size;
volatile uint16 n,max=2000,min=0;
uint8 Mensaje;
#define SlaveAddress 0x23



void setROJO(float degrees){
    float val;
    val=255*(degrees);
    PWM_R_WriteCompare(val);
}

void setVERDE(float degrees){
    float val;
    val=255*(degrees);
    PWM_G_WriteCompare(val);
}

void setAZUL(float degrees){
    float val;
    val=255*(degrees);
    PWM_B_WriteCompare(val);
}

CY_ISR(Int_Timer){
            Timer_1_Sleep();
    
        
        LCD_Position(1,0);
        LCD_PrintString("C:"); 
        
        if(90<p&&p<=100){
            setROJO(1);
            setVERDE(0);
            setAZUL(0);
            
            LCD_Position(1,2);
            LCD_PrintString("ROJO");
            RA=9;
        }
         if(80<p&&p<=90){
            setROJO(1);
            setVERDE(0);
            setAZUL(1);
          
            LCD_Position(1,2);
            LCD_PrintString("MORADO");
            RA=8;
        }
         if(70<p&&p<=80){
            setROJO(0);
            setVERDE(0);
            setAZUL(1);
            
            LCD_Position(1,2);
            LCD_PrintString("AZUL");
            RA=7;
        }
          if(60<p&&p<=70){
            setROJO(0.1);
            setVERDE(0.7);
            setAZUL(0.1);
            
            LCD_Position(1,2);
            LCD_PrintString("VERDE OSCURO");
            RA=6;
        }
         if(50<p&&p<=60){
            setROJO(0.7);
            setVERDE(0.6);
            setAZUL(0.7);
            
            LCD_Position(1,2);
            LCD_PrintString("ROSADO");
            RA=5;
        }
         if(40<p&&p<=50){
            setROJO(0.9);
            setVERDE(0.7);
            setAZUL(0.6);
            
            LCD_Position(1,2);
            LCD_PrintString("NARANJA");
            RA=4;
        }
         if(30<p&&p<=40){
            setROJO(1-0);
            setVERDE(1-0);
            setAZUL(1-1);
            
            LCD_Position(1,2);
            LCD_PrintString("AMARILLO");
            RA=3;
        }
         if(20<p&&p<=30){
            setROJO(1-1);
            setVERDE(1);
            setAZUL(1-1);
            
            LCD_Position(1,2);
            LCD_PrintString("VERDE");
            RA=2;
        }
         if(10<p&&p<=20){
            setROJO(1-1);
            setVERDE(1-0);
            setAZUL(1-0);
            
            LCD_Position(1,2);
            LCD_PrintString("CIAN");
            RA=1;
        }
         if(p<=10){
            setROJO(1-0);
            setVERDE(1-0);
            setAZUL(1-0);
            
            LCD_Position(1,2);
            LCD_PrintString("BLANCO");
            RA=0;
            
                    
            
            
         }
        if(p>100){
            setROJO(1);
            setVERDE(0);
            setAZUL(0);
            
            LCD_Position(1,2);
            LCD_PrintString("ROJO");
            RA=11;
        }
        
        LCD_Position(0,12);
        LCD_PrintNumber(RA);
            
            
            size=UART_BL_GetRxBufferSize(); //Leer la cantidad de datos recibidos
            
            UART_BL_PutString("L,");
            dato=n/10000;
            n=n%10000;
            UART_BL_PutChar(dato+0x30);
            dato=n/1000;
            n=n%1000;
            UART_BL_PutChar(dato+0x30);
            dato=n/100;
            n=n%100;
            UART_BL_PutChar(dato+0x30); 
            dato=n/10;
            n=n%10;
            UART_BL_PutChar(dato+0x30);
            dato=n;
            n=n%10;
            UART_BL_PutChar(dato+0x30);
            
            UART_BL_PutString(",");
            UART_BL_PutString("C,");
            
            dato=RA/10;
            RA=RA%10;
            UART_BL_PutChar(dato+0x30);
            dato=RA;
            RA=RA%10;
            UART_BL_PutChar(dato+0x30);
            
            
            UART_BL_PutString(",");
           
            
            if(size>0){ //Si se recibieron caracteres se verifica si se solcitó riego
             
                    Mensaje=UART_BL_GetChar();
                    Mensaje=Mensaje-0x30;
            }
            
            
            LCD_Position(0,14);
            LCD_PrintNumber(Mensaje);
            
            
            
            Timer_1_Wakeup();     
}

int main(void)
{
    CyGlobalIntEnable;
    ISR_Timer_StartEx(Int_Timer);
    Timer_1_Start();
    LCD_Start();
    I2C_Start();
    ISR_Timer_StartEx(Int_Timer);
    PWM_R_Start();
    PWM_G_Start();
    PWM_B_Start();
    Clock_1_Start();
    
    UART_BL_Start();
    
    
    
    

    for(;;)
    {
       // LCD_ClearDisplay();
        do{
            status=I2C_MasterSendStart(SlaveAddress,I2C_WRITE_XFER_MODE);
             
        }while(I2C_MSTR_NO_ERROR!=status);
        for(i=0;i<2;i++){
            status=I2C_MasterWriteByte(b[i]);
            if(status!=I2C_MSTR_NO_ERROR){
                break;
            }
            
            status=I2C_MasterSendRestart(SlaveAddress,I2C_WRITE_XFER_MODE);
            if(status!=I2C_MSTR_NO_ERROR){
                break;
            }
            
        }
        
        CyDelay(180);
        do{
            status=I2C_MasterSendRestart(SlaveAddress,I2C_READ_XFER_MODE);
        }while(I2C_MSTR_NO_ERROR!=status);
        h=I2C_MasterReadByte(I2C_ACK_DATA);
        l=I2C_MasterReadByte(I2C_NAK_DATA);
       
       
        I2C_MasterSendStop();
         h=h<<8;
        //l=l>>8;
        n=(h+l)/1.2;
        
        switch (Mensaje){
            case 0:
                max=2000;
            break;
            case 1:
                max=1800;   
            break;
            case 2:
                max=1600;
            break;
            case 3:
                max=1400;
            break;
            case 4:
                max=1200;
            break;
            case 5:
                max=1000;
            break;
            case 6:
                max=800;
            break;
            case 7:
                max=600;
            break;
            case 8:
                max=400;
            break;
            case 9:
                max=200;
            break;
            
        
        }
        
        LCD_Position(1,12);
        LCD_PrintNumber(max);
        
        p=(n*100)/max;
        
        if(n<min){
            p=0;
            Buzzer_Write(1);
            
            
        }else if(n>max){
            p=101;
            Buzzer_Write(1);
            
            
        }else{
            Buzzer_Write(0);
        }
        
        LCD_Position(0,6);
        LCD_PrintString("%:");
        LCD_Position(0,8);
        LCD_PrintNumber(p);
        
        LCD_Position(0,0);
        LCD_PrintString("L:");
        LCD_Position(0,2);
        LCD_PrintNumber(n);
        
         CyDelay(1000);
        LCD_ClearDisplay();
        
        
        
        
        
   
    }
}

/* [] END OF FILE */
