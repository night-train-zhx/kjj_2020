#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"
#include "timer.h"
#include "beep.h"
#define   cm 0
#define SIZE 4
#define SIZE_f 4
/************************************************
 ALIENTEK精英STM32开发板实验26
 触摸屏 实验    
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   





typedef struct _dian
    {
			int X;
			int Y;
			int co;           //1:black  0:white   
			int an;           //1：被点过 2：未被点过
    }dian;

		dian Dian[6][4];

char jd[100],show[5],mode=0,qishi=0,success=3,si=0,sisi=0;
int i=0,j=0,ii=0,rd=0,im=0,jm=0,rdm=0,ti_min=0,tt=0,ti=0,tj=0,diyi=0,aaa=0,bbb=0,ccc=0,ddd=0,
	count_10ms=0,count_10ms_2=0,cci=0,count_2=0,count_3=0,mode_c=0,mode_chan=0,geshu=0,shu_10=0,
beep_ks=0,x_c=0,y_c=0,count_4=0,This_1=0,t=0,aa[10],bb[10],cc[10],dd[10],fen[10],order[10],count_6=0,count_7=0,bs=0,
q=0,k=0,p=0;
long count_1=0;
extern u32 time_us;
extern int sec_x,sec_z,sec_xx;
u32 FLASH_SIZE; 
u8 ceshi_3[310],ren_1[10][4],T[10],F[10][4],RR[10][4],fen_2[10][2],ceshi_1[10],ceshi_4[10],fs_1[4],fs_2[2],ceshi_2[4];
unsigned char ID[6];
		
void jd_init(void)
{
	int di=0;
	for(di=0;di<=99;di++)
	{
		jd[di]=rand()%4;
	}
}


void xiangxia_1(int i,int j, int zhi)
{
	if(Dian[i][j].co==0)
	{
		Dian[i][j].Y+=zhi;
	  LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y+136,Dian[i][j].X+119,Dian[i][j].Y+136+3,BLACK);
	  LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y,Dian[i][j].X+119,Dian[i][j].Y+135,WHITE);
	}
	else if(Dian[i][j].co==1)
	{
		Dian[i][j].Y+=zhi;
	  LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y+136,Dian[i][j].X+119,Dian[i][j].Y+136+3,BLACK);
	  LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y,Dian[i][j].X+119,Dian[i][j].Y+135,BLACK);
	}
}

void xiangxia_2(int i,int j, int zhi)
{
	if(Dian[i][j].co==0)
	{
		Dian[i][j].Y+=zhi;
	  LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y+131-zhi,Dian[i][j].X+119,Dian[i][j].Y+135-zhi+4,WHITE);
	}
	if(Dian[i][j].co==1)
	{
		Dian[i][j].Y+=zhi;
	  LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y-zhi-4,Dian[i][j].X+119,Dian[i][j].Y-1-4,WHITE);
	}
}

void xiangxia_2_2(int i,int j, int zhi)
{
  if(Dian[i][j].co==1)
	{
		//LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y+136-zhi+4,Dian[i][j].X+119,Dian[i][j].Y+136+3-zhi+4,BLACK);
		LCD_Fill_zhx(Dian[i][j].X+4,Dian[i][j].Y+136-zhi+4,Dian[i][j].X+119,Dian[i][j].Y+136+3+4,BLACK);
	}
}

void chushi(void)
{
	for(im=0;im<=5;im++)
  {
		//srand(time_us);
    rdm=rand()%4;
		for(jm=0;jm<=3;jm++)
	  {
			if(jm==rdm)
			{
				Dian[im][jm].co=1;
			}
			else
			{
				Dian[im][jm].co=0;
			}
			xiangxia_1(im,jm,0);
		}
  }	
}

void fill_cla(int ci,int cj)
{
	if(ci<=5)
	 LCD_Fill_zhx(119*ci+4,140*cj,119*ci+118,140*cj+136,BLACK);
}
void fill_cla_lv(int ci,int cj)
{
	if(ci<=5)
	 LCD_Fill_zhx(119*ci+4,140*cj,119*ci+118,140*cj+136,GREEN);
}
void fill_cla_qian(int ci,int cj)
{
	if(ci<=5)
	 LCD_Fill_zhx(119*ci+4,140*cj,119*ci+118,140*cj+136,GRAYBLUE);
}
void fill_cla_zong(int ci,int cj)
{
	if(ci<=5)
	 LCD_Fill_zhx(119*ci+4,140*cj,119*ci+118,140*cj+136,BRRED);
}
void fill_cla_hui(int ci,int cj)
{
	if(ci<=5)
	 LCD_Fill_zhx(119*ci+4,140*cj,119*ci+118,140*cj+136,GRAY);
}

void chushi_classic(int diyi)
{
	for(cci=0;cci<=4;cci++)
	{
	  show[4-cci]=jd[diyi+cci];
		if(cci!=0)
		{
		  fill_cla(show[4-cci],4-cci);
		}
		LCD_ShowNum(300,705+17*cci,show[4-cci],1,16);
	}
}

int findmin_y(void)
{
	int t=0;
	int min=700;
	int min_t=0;
	for(t=0;t<=5;t++)
	{
		if((Dian[t][0].Y<=min)&&(Dian[t][0].Y>=0))
		{
			min=Dian[t][0].Y;
			min_t=t;
		}
	}
	return min_t;
}



int main(void)
{	 		    
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	TIM3_Int_Init(899,7199);   //20ms
	TIM2_Int_Init(99,7199);   //10ms
	time4_init(356,0);
	TIM1_PWM_Init(999,71);
	TIM_Cmd(TIM4, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM1, DISABLE);
	LED_Init();			     //LED端口初始化
	LCD_Init();		 	
	W25QXX_Init();
	//LCD_Display_Dir(0);
	tp_dev.init();
	FLASH_SIZE=128*1024*1024;	//FLASH 大小为16M字节
	POINT_COLOR=RED;//设置字体为红色 
	
	while(W25QXX_ReadID()!=W25Q128)								//检测不到W25Q128
	{
		LCD_ShowString(30,150,200,16,16,"W25Q128 Check Failed!");
		delay_ms(500);
		LCD_ShowString(30,150,200,16,16,"Please Check!        ");
		delay_ms(500);
		LED0=!LED0;//DS0闪烁
	}
	
	
//	for(i=0;i<=309;i++)
//	{
//		ceshi_3[i]=0;
//	}
//	W25QXX_Write((u8*)ceshi_3,FLASH_SIZE-310,310);
	
	ks:
	clear(WHITE);
	for(i=0;i<=5;i++)
	{
		Dian[i][0].X=0;
		Dian[i][1].X=119;
		Dian[i][2].X=238;
		Dian[i][3].X=357;
	}
	for(i=0;i<=3;i++)
	{
		Dian[0][i].Y=-140;
		Dian[1][i].Y=0;
		Dian[2][i].Y=140;
		Dian[3][i].Y=280;
		Dian[4][i].Y=420;
		Dian[5][i].Y=560;
	}
	for(i=0;i<=5;i++)
	{
		for(j=0;j<=3;j++)
	{
		Dian[i][j].co=0;
		Dian[i][j].an=0;
	}
	}
	for(i=0;i<=4;i++)
	{
		show[i]=0;
	}
	for(i=0;i<=9;i++)
	 {
		 ceshi_1[i]=0;
	 }

	 for(i=0;i<=9;i++)
	 {
		 ceshi_2[i]=0;
	 }
	 for(i=0;i<=9;i++)
	 {
		 ceshi_4[i]=0;
	 }
	 for(i=0;i<=9;i++)
	{
		for(j=0;j<=1;j++)
	{
		fen_2[i][j]=0;
	}
	}
	count_1=0;
	count_2=0;
	count_4=0;
	 count_6=0;
	 count_7=0;
	si=0;
	sisi=0;
	diyi=0;
	cci=0;
	geshu=0;
	mode_c=0;
	beep_ks=0;
	x_c=0;
	y_c=0;
	//success=3;
	i=0;
	j=0;
	bs=0;
	mode=3;
	count_3=0;
	TIM3_Int_Init(999,7199);
	TIM_Cmd(TIM4, DISABLE);
	TIM_Cmd(TIM2, DISABLE);
	TIM_Cmd(TIM3, DISABLE);
	TIM_Cmd(TIM1, DISABLE);
	
	LCD_Fill(100,100,300,250,BLUE);
	LCD_Fill(100,350,300,500,BLUE);
	LCD_ShowString(100,100,90,16,16,"play");
	LCD_ShowString(100,350,90,16,16,"rank");
	
	
	
	
	
	while(1)
	{
		tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)         //play
		{
			
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
			clear(WHITE);
			
      LCD_ShowString(100,20,300,16,16,"put in your 6wei ID");
			 LCD_DrawRectangle(80,80,140,140);
			 LCD_DrawRectangle(180,80,240,140);
			 LCD_DrawRectangle(280,80,340,140);
			 LCD_DrawRectangle(380,80,440,140);
			 LCD_Fill(50,200,150,300,YELLOW);
			 LCD_ShowString(50,200,33,16,16,"1");
			 LCD_Fill(180,200,280,300,YELLOW);
			 LCD_ShowString(180,200,33,16,16,"2");
			 LCD_Fill(310,200,410,300,YELLOW);
			 LCD_ShowString(310,200,33,16,16,"3");
			 LCD_Fill(50,330,150,430,YELLOW);
			 LCD_ShowString(50,330,33,16,16,"4");
			 LCD_Fill(180,330,280,430,YELLOW);
			 LCD_ShowString(180,330,33,16,16,"5");
			 LCD_Fill(310,330,410,430,YELLOW);
			 LCD_ShowString(310,330,33,16,16,"6");
			 LCD_Fill(50,460,150,560,YELLOW);
			 LCD_ShowString(50,560,33,16,16,"7");
			 LCD_Fill(180,460,280,560,YELLOW);
			 LCD_ShowString(180,460,33,16,16,"8");
			 LCD_Fill(310,460,410,560,YELLOW);
			 LCD_ShowString(310,460,33,16,16,"9");
			 LCD_Fill(180,590,280,690,YELLOW);
			 LCD_ShowString(180,590,33,16,16,"0");
			 LCD_Fill(310,590,410,690,GREEN);
			 LCD_ShowString(310,590,70,16,16,"NEXT");
			 ID[3]=123;
			 goto denglu;
		                             //////////////////////////////////////////////////////////////////////////////////
		}
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>350&&tp_dev.y[cm]<500)         //rank
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>350&&tp_dev.y[cm]<500)) break;
	     }
			clear(WHITE);
			clear(BLUE);
			 
			 
			W25QXX_Read(T,FLASH_SIZE-300,10);           //读 T[i]
			 
			 for(i=0;i<=9;i++)                    //     第一个不为1的数是本次要写的位置 
			 {
				 if(T[i]!='1')
				 {
					 q=i;
					 break;
				 }
			 } 
			 
			 if(q==0)
				 goto ks;
			 
			 for(i=0;i<=(q-1);i++)   
       {			 
				 W25QXX_Read(F[i],FLASH_SIZE-(230-i*20),SIZE_f);
			 }
			 
			 for(i=0;i<=(q-1);i++)                    
			 {
				 aa[i]=F[i][0]-'0';
				 bb[i]=F[i][1]-'0';
				 cc[i]=F[i][2]-'0';
				 dd[i]=F[i][3]-'0';
			 }
			 
			 for(i=0;i<=(q-1);i++)                    
			 {
				 fen[i]=aa[i]*1000+bb[i]*100+cc[i]*10+dd[i]*1;
			 }
			 
			 for(i=0;i<=(q-1);i++)                    
			 {
				 order[i]=i;
			 }
			 
			 if(q==1)
			 {
				 W25QXX_Read(ceshi_1,FLASH_SIZE-240,SIZE);
				 LCD_ShowString(120,120,96,16,16,ceshi_1);
				 W25QXX_Read(ceshi_4,FLASH_SIZE-230,SIZE_f);
				 LCD_ShowString(300,120,70,16,16,ceshi_4);
			 }
			 
			 else
			 {
			 for(i=0;i<=(q-2);i++)                    //    排序
			 {
				 k=i;
				 for(j=i+1;j<=(q-1);j++)
				 {
					 if(fen[k]<fen[j])
					 {
						k=j; 
					 }
					 if(k!=i)
					 {
						 p=fen[k];
						 fen[k]=fen[i];
						 fen[i]=p;
						 p=order[k];
						 order[k]=order[i];
						 order[i]=p;
					 }
				 }
			 }
			 
			 for(i=0;i<=(q-1);i++)                    
			 {
			   W25QXX_Read(RR[i],FLASH_SIZE-(240-i*20),SIZE);
				 W25QXX_Read(fen_2[i],FLASH_SIZE-(235-i*20),2);
			 }
			 
			 
			 for(i=0;i<=(q-1);i++)                    
			 {
				 LCD_ShowString(120,(120+(q-i)*18),32,16,16,RR[(order[i])]);
				// LCD_ShowString(240,(120+(q-i)*18),48,16,16,fen_2[(order[i])]);
			   LCD_ShowNum(300,(120+(q-i)*18),fen[i],4,16);
			 }
		 }
			 
//			 W25QXX_Read(iid_1,FLASH_SIZE-100,SIZE);
//			 W25QXX_Read(FS1,FLASH_SIZE-50,SIZE_f);
//			 LCD_ShowString(100,200,300,16,16,iid_1);
//			 LCD_ShowString(100,220,300,16,16,FS1);
			 LCD_Fill(40,560,140,660,YELLOW);
		 //LCD_ShowString(40,60,70,16,16,ceshi_4);
			 while(1)
		{
			
			tp_dev.scan(0);
		if(tp_dev.x[cm]>40&&tp_dev.x[cm]<140&&tp_dev.y[cm]>560&&tp_dev.y[cm]<660)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>40&&tp_dev.x[cm]<140&&tp_dev.y[cm]>560&&tp_dev.y[cm]<660)) break;
	     }
			
       TIM_Cmd(TIM1, DISABLE);                
			 goto ks;
		}

		}
                             //////////////////////////////////////////////////////////////////////////////////
		}
		
	}
	
	
	denglu:
	while(1)
	 {
		 if(ID[3]==123)
		 {
			 tp_dev.scan(0);
	    if(tp_dev.x[cm]>50&&tp_dev.x[cm]<150&&tp_dev.y[cm]>200&&tp_dev.y[cm]<300)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>50&&tp_dev.x[cm]<150&&tp_dev.y[cm]>200&&tp_dev.y[cm]<300)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='1';
					  LCD_ShowString((count_4*100),90,35,16,16,"1");
		  }
			if(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>200&&tp_dev.y[cm]<300)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>200&&tp_dev.y[cm]<300)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='2';
					  LCD_ShowString((count_4*100),90,35,16,16,"2");
		  }
			if(tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>200&&tp_dev.y[cm]<300)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>200&&tp_dev.y[cm]<300)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='3';
					  LCD_ShowString((count_4*100),90,35,16,16,"3");
		  }
			if(tp_dev.x[cm]>50&&tp_dev.x[cm]<150&&tp_dev.y[cm]>330&&tp_dev.y[cm]<430)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>50&&tp_dev.x[cm]<150&&tp_dev.y[cm]>330&&tp_dev.y[cm]<430)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='4';
					  LCD_ShowString((count_4*100),90,35,16,16,"4");
		  }
			if(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>330&&tp_dev.y[cm]<430)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>330&&tp_dev.y[cm]<430)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='5';
					  LCD_ShowString((count_4*100),90,35,16,16,"5");
		  }
			if(tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>330&&tp_dev.y[cm]<430)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>330&&tp_dev.y[cm]<430)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='6';
					  LCD_ShowString((count_4*100),90,35,16,16,"6");
		  }
			if(tp_dev.x[cm]>50&&tp_dev.x[cm]<150&&tp_dev.y[cm]>460&&tp_dev.y[cm]<560)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>50&&tp_dev.x[cm]<150&&tp_dev.y[cm]>460&&tp_dev.y[cm]<560)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='7';
					  LCD_ShowString((count_4*100),90,35,16,16,"7");
		  }
			if(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>460&&tp_dev.y[cm]<560)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>460&&tp_dev.y[cm]<560)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='8';
					  LCD_ShowString((count_4*100),90,35,16,16,"8");
		  }
			if(tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>460&&tp_dev.y[cm]<560)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>460&&tp_dev.y[cm]<560)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='9';
					  LCD_ShowString((count_4*100),90,35,16,16,"9");
		  }
			if(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>590&&tp_dev.y[cm]<690)
		  {
				while(1)
		    {
			      tp_dev.scan(0);
					if(!(tp_dev.x[cm]>180&&tp_dev.x[cm]<280&&tp_dev.y[cm]>590&&tp_dev.y[cm]<690)) break;
				}
					  count_4++;
					  ID[(count_4-1)]='0';
					  LCD_ShowString((count_4*100),90,35,16,16,"0");
		  }
			
		}
		 
		
		tp_dev.scan(0);
	   if((tp_dev.x[cm]>310&&tp_dev.x[cm]<410&&tp_dev.y[cm]>590&&tp_dev.y[cm]<690)&&(ID[3]!=123))
		 {
			 // id
			clear(WHITE);
			 
		  W25QXX_Read(T,FLASH_SIZE-300,10);           //读 T[i]
			 
			 for(i=0;i<=9;i++)                    //     第一个不为1的数是本次要写的位置 
			 {
				 if(T[i]!='1')
				 {
					 t=i;
					 break;
				 }
			 }
			 
			 for(i=0;i<=9;i++)
			 {
				 W25QXX_Read(ren_1[i],FLASH_SIZE-(240-i*20),SIZE);
			 }
			 
			 if(t==0)
				 i=t;
			 else
			 {
			 for(i=0;i<=(t-1);i++)
			  {
				 if((ID[0]==ren_1[i][0])&&(ID[1]==ren_1[i][1])&&(ID[2]==ren_1[i][2])&&(ID[3]==ren_1[i][3]))
				 {
					 This_1=i;
			     break;
				 }
			  }
		   }
			 
			 if(i==t)
			 {
				 This_1=t;
				 T[t]='1';
				 W25QXX_Write((u8*)ID,FLASH_SIZE-(240-t*20),SIZE);
				 W25QXX_Write((u8*)T,FLASH_SIZE-300,10);
				 t=0;
			 }
				 
			//LCD_ShowString(0,20,100,16,16,ID);
//      LCD_DrawLine(120,0,120,320);
//	    LCD_DrawLine(360,0,360,320);
//	    LCD_Fill(220,270,260,290,BLUE);
//	    LCD_Fill(0,304,480,320,BLACK);
//	    LCD_Fill(200,100,280,180,GREEN);
			 
			 
			  LCD_Fill(100,100,300,250,BLUE);
	 LCD_Fill(100,350,300,500,BLUE);
	 LCD_Fill(100,600,300,750,BLUE);
	 LCD_ShowString(100,100,90,16,16,"classic");
	 LCD_ShowString(100,350,90,16,16,"jieji");
	 LCD_ShowString(100,600,90,16,16,"chan");

			 goto play;
			 
			 
		 }
		
	 }
	 
	 
	 
	
	play:
	while(1)
	{
		tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)         //classic
		{
			
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
			clear(WHITE);
			
      LCD_Fill(100,200,300,400,BLUE);
	    LCD_Fill(100,500,300,700,BLUE);
			LCD_ShowString(100,200,90,16,16,"25");
	    LCD_ShowString(100,500,90,16,16,"50");
			jd_init();
//			TIM_Cmd(TIM4, ENABLE);
//			TIM_Cmd(TIM2, ENABLE);
//			TIM_Cmd(TIM3, DISABLE);
			TIM_Cmd(TIM4, ENABLE);
			while(1)
			{
				tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>200&&tp_dev.y[cm]<400)           //25
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>200&&tp_dev.y[cm]<400)) break;
	     }
			clear(WHITE);
			//TIM_Cmd(TIM4, ENABLE);
			mode_c=25;
			goto classic;
		}
				else if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>500&&tp_dev.y[cm]<700)/////////50
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>500&&tp_dev.y[cm]<700)) break;
	     }
			clear(WHITE);
			//TIM_Cmd(TIM4, ENABLE);
      mode_c=50;
			goto classic;                   //////////////////////////////////////////////////////////////////////////////////
		}
			}                             //////////////////////////////////////////////////////////////////////////////////
		}
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>350&&tp_dev.y[cm]<500)         //jieji
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>350&&tp_dev.y[cm]<500)) break;
	     }
			clear(WHITE);
			LCD_Fill_zhx(0,0,3,700,BLACK);
			LCD_Fill_zhx(119,0,122,700,BLACK);
			LCD_Fill_zhx(238,0,241,700,BLACK);
			LCD_Fill_zhx(357,0,360,700,BLACK);
			LCD_Fill_zhx(476,0,479,700,BLACK);
			LCD_Fill(100,100,300,250,GREEN);
			LCD_ShowString(100,200,90,16,16,"start");
      TIM_Cmd(TIM3, ENABLE);
			goto jieji;                              //////////////////////////////////////////////////////////////////////////////////
		}
		
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>600&&tp_dev.y[cm]<750)         //chan
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>600&&tp_dev.y[cm]<750)) break;
	     }
			clear(WHITE);
			
      LCD_Fill(100,200,300,400,BLUE);
	    LCD_Fill(100,500,300,700,BLUE);
			LCD_ShowString(100,200,90,16,16,"10sec");
	    LCD_ShowString(100,500,90,16,16,"20sec");
			jd_init();
//			TIM_Cmd(TIM4, ENABLE);
//			TIM_Cmd(TIM2, ENABLE);
//			TIM_Cmd(TIM3, DISABLE);
			TIM_Cmd(TIM4, ENABLE);
			while(1)
			{
				tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>200&&tp_dev.y[cm]<400)           //10
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>200&&tp_dev.y[cm]<400)) break;
	     }
			clear(WHITE);
			//TIM_Cmd(TIM4, ENABLE);
			mode_chan=10;
			goto chan;
		}
				else if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>500&&tp_dev.y[cm]<700)/////////20
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>500&&tp_dev.y[cm]<700)) break;
	     }
			clear(WHITE);
			//TIM_Cmd(TIM4, ENABLE);
      mode_chan=20;
			goto chan;                   //////////////////////////////////////////////////////////////////////////////////
		}
			}                              //////////////////////////////////////////////////////////////////////////////////
		}
	}
	
	classic:

			clear(WHITE);
			srand(time_us);
			diyi=rand()%48;
	    TIM_Cmd(TIM4, DISABLE);
			LCD_ShowNum(0,710,diyi,1,16);
			chushi_classic(diyi);
			LCD_ShowString(show[3]*140,420,90,16,16,"start");
			while(1)
	//for(cci=5;cci<=mode_c;cci++)
			{
				LCD_Fill(0,750,200,799,YELLOW);
				LCD_Fill(280,750,479,799,BROWN);
				
				//while(tp_dev.scan(0)==0);
				if(count_10ms>=10+beep_ks)
				{
					beep_ks=0;
					TIM_Cmd(TIM1, DISABLE);
				}
				if(tp_dev.scan(0)==1)
			{
				
					
				
				
				if(tp_dev.x[cm]>280&&tp_dev.x[cm]<479&&tp_dev.y[cm]>750&&tp_dev.y[cm]<799)
				{
					while(1)
		      {
			      tp_dev.scan(0);
					  if(!(tp_dev.x[cm]>280&&tp_dev.x[cm]<479&&tp_dev.y[cm]>750&&tp_dev.y[cm]<799)) break;
				  }
					
					goto ks;
					
				}
				
				
				
				else if((tp_dev.x[cm]>show[3]*140-20)&&(tp_dev.x[cm]<show[3]*140+115+20)&&(tp_dev.y[cm]>420-20)&&(tp_dev.y[cm]<560-20))
				{
					while(1)
		      {
			      tp_dev.scan(0);
					  if(!((tp_dev.x[cm]>show[3]*140-20)&&(tp_dev.x[cm]<show[3]*140+115+20)&&(tp_dev.y[cm]>420-20)&&(tp_dev.y[cm]<560-20))) break;
				  }
					if(count_2==0)
					{
						count_10ms=0;
						TIM_Cmd(TIM2, ENABLE);
					}
					
					if(song[count_6]==0xff)
						count_6=0;
					TIM_Cmd(TIM1, ENABLE);
					change_fre(song[count_6]);
					beep_ks=count_10ms;
					count_6++;
					LCD_Clear(WHITE);
					show[4]=show[3];
					show[3]=show[2];
					show[2]=show[1];
					show[1]=show[0];
					if(count_2<=mode_c-5)
					  show[0]=jd[diyi+5+count_2];
					else
						show[0]=6;
					for(i=0;i<=4;i++)
					{
						if(i==0)
							fill_cla_hui(show[4-i],4-i);
						else
					    fill_cla(show[4-i],4-i);
					 //LCD_ShowNum(300,705+17*i,show[4-i],1,16);
					}
					
					count_2++;
					if(count_2>=mode_c)
						goto succ;
//						break;
				}
				else
		{
			x_c=tp_dev.x[cm]/119;
			y_c=tp_dev.y[cm]/119;
			TIM_Cmd(TIM1, ENABLE);
			change_fre(400);
			for(ii=0;ii<=4;ii++)
						{
						LCD_Fill(x_c*119,y_c*140,x_c*119+119,y_c*140+140,RED);
						delay_ms(100);
						LCD_Fill(x_c*119,y_c*140,x_c*119+119,y_c*140+140,WHITE);
					  delay_ms(100);
						}
						success=0;
						TIM_Cmd(TIM1, DISABLE);
						goto fail;
		}
			}
			}
			
			succ:		

			  LCD_Fill(100,100,300,250,BLUE);
				LCD_ShowString(100,100,90,16,16,"back");
			//sec_s=count_10ms/1000;
			count_10ms_2=count_10ms;
			fs_1[0]=(count_10ms_2/1000)+'0';
			// jl_2=jl_2%1000;
			 fs_1[1]=((count_10ms_2%1000)/100)+'0';
			// jl_2=jl_2%100;
			 fs_1[2]=(((count_10ms_2%1000)%100)/10)+'0';
			// jl_2=jl_2%10;
			 fs_1[3]=((((count_10ms_2%1000)%100)%10))+'0';
			 
			 LCD_ShowString(100,20,90,16,16,fs_1);
			 
			 W25QXX_Read(ceshi_2,FLASH_SIZE-(230-This_1*20),SIZE_f);
			 if(!((ceshi_2[3]>='0')&&(ceshi_2[3]<='9')))
			 {
				  W25QXX_Write((u8*)fs_1,FLASH_SIZE-(230-This_1*20),SIZE_f);
			 }
			 
			 else
			 {
				 aaa=ceshi_2[0]-'0';
				 bbb=ceshi_2[1]-'0';
				 ccc=ceshi_2[2]-'0';
				 ddd=ceshi_2[3]-'0';
		shu_10=aaa*1000+bbb*100+ccc*10+ddd*1;		
       if(count_10ms_2<shu_10)
			 {				 

			 W25QXX_Write((u8*)fs_1,FLASH_SIZE-(230-This_1*20),SIZE_f);			//从倒数第100个地址处开始,写入SIZE长度的数据
				 count_10ms_2=0;
			 }
			 }
			
		while(1)
		{
				
//			if(tp_dev.scan(0)==1)
//			{
			tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
       TIM_Cmd(TIM1, DISABLE);                      //////////////////////////////////////////////////////////////////////////////////
			 goto ks;
		}

		}
		
		
		fail:
		LCD_Clear(WHITE);
		LCD_ShowString(100,10,90,16,16,"fail");
		LCD_Fill(100,100,300,250,BLUE);
		LCD_ShowString(100,100,90,16,16,"back");
		while(1)
		{
			tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
       TIM_Cmd(TIM1, DISABLE);                      //////////////////////////////////////////////////////////////////////////////////
			 goto ks;
		}
	}


	 
	jieji:
	while(1)
	{
		tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
			clear(WHITE);
      mode=0;
      TIM_Cmd(TIM4, ENABLE);
//			while(1);
//			goto jieji;                              //////////////////////////////////////////////////////////////////////////////////
			 break;
		}
	}
	while(1)
	{
		LCD_Fill(0,753,200,795,YELLOW);
		LCD_Fill(280,753,479,795,BROWN);
		if(sisi==1)
		{
			geshu=count_3-7;
			mode=3;
			TIM_Cmd(TIM4, DISABLE);
			for(ii=0;ii<=4;ii++)
						{
						LCD_Fill(0,702,479,710,RED);
						delay_ms(100);
						LCD_Fill(0,702,479,710,WHITE);
					  delay_ms(100);
						}
						sisi=0;
						LCD_Clear(WHITE);
						LCD_Fill(100,100,300,250,BLUE);
						LCD_ShowString(100,100,90,16,16,"back");
						LCD_ShowNum(250,710,geshu,3,16);
						break;
		}
		if(tp_dev.scan(0)==1)
			{	
				
				if(tp_dev.x[cm]>0&&tp_dev.x[cm]<200&&tp_dev.y[cm]>750&&tp_dev.y[cm]<799)
				{
					while(1)
		      {
			      tp_dev.scan(0);
					  if(!(tp_dev.x[cm]>0&&tp_dev.x[cm]<200&&tp_dev.y[cm]>750&&tp_dev.y[cm]<799)) break;
				  }
					count_7++;
					if(count_7%2==1)
					  TIM_Cmd(TIM3, DISABLE);
					else
						TIM_Cmd(TIM3, ENABLE);
				}
				
				
//				else if(tp_dev.x[cm]>280&&tp_dev.x[cm]<479&&tp_dev.y[cm]>750&&tp_dev.y[cm]<799)
//				{
//					while(1)
//		      {
//			      tp_dev.scan(0);
//					  if(!(tp_dev.x[cm]>280&&tp_dev.x[cm]<479&&tp_dev.y[cm]>750&&tp_dev.y[cm]<799)) break;
//				  }
//					
//					goto ks;
//					
//				}
				
				
				 else if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<700)
			  {	
//					while(1)
//		      {
//			      tp_dev.scan(0);
//					  if(!(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<700)) break;
//				  }
				  ti_min=findmin_y();
					tt=(tp_dev.y[0]-Dian[ti_min][0].Y)/140;
					ti=ti_min+tt;
					if(ti>=6)
					{
						ti=ti-6;
					}
					tj=tp_dev.x[0]/119;
					if(Dian[ti][tj].co==1)
					{
						Dian[ti][tj].an=1;
						TIM_Cmd(TIM1, ENABLE);
			      change_fre(rand()%1000+900);
						delay_ms(50);
						TIM_Cmd(TIM1, DISABLE);
						//count_4++;
					}
					else
					{
						geshu=count_3-2;
						Dian[ti][tj].an=2;
			      mode=3;
						TIM_Cmd(TIM4, DISABLE);
						//LCD_Fill(100,710,200,770,RED);     //ceshi
						
						TIM_Cmd(TIM1, ENABLE);
			      change_fre(400);
						for(ii=0;ii<=4;ii++)
						{
						LCD_Fill(Dian[ti][tj].X,Dian[ti][tj].Y,Dian[ti][tj].X+119,Dian[ti][tj].Y+140,RED);
						delay_ms(100);
						LCD_Fill(Dian[ti][tj].X,Dian[ti][tj].Y,Dian[ti][tj].X+119,Dian[ti][tj].Y+140,WHITE);
					  delay_ms(100);
						}
						TIM_Cmd(TIM1, DISABLE);
						LCD_Clear(WHITE);
						LCD_Fill(100,100,300,250,BLUE);
						LCD_ShowString(100,100,90,16,16,"back");
						LCD_ShowNum(250,710,geshu,3,16);
						fs_2[0]=geshu/10;
						fs_2[1]=geshu%10;
						
						
						W25QXX_Read(ceshi_2,FLASH_SIZE-(235-This_1*20),2);
			 if(!((ceshi_2[1]>='0')&&(ceshi_2[1]<='9')))
			 {
				  W25QXX_Write((u8*)fs_2,FLASH_SIZE-(235-This_1*20),2);
			 }
			 
			 else
			 {
				 aaa=ceshi_2[0]-'0';
				 bbb=ceshi_2[1]-'0';
				 
		shu_10=aaa*10+bbb*1;		
       if(geshu>shu_10)
			 {				 

			 W25QXX_Write((u8*)fs_2,FLASH_SIZE-(235-This_1*20),2);			//从倒数第100个地址处开始,写入SIZE长度的数据
				 geshu=0;
			 }
			 }
						
						break;
					}
			  }
			}
	}
	 
	while(1)
	{
		tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
                             //////////////////////////////////////////////////////////////////////////////////
			 goto ks;
		}
	}
	
	
	chan:
	clear(WHITE);
			srand(time_us);
			diyi=rand()%48;
	    TIM_Cmd(TIM4, DISABLE);
			LCD_ShowNum(0,710,diyi,1,16);
			chushi_classic(diyi);
			LCD_ShowString(show[3]*140,420,90,16,16,"start");
			while(1)
	//for(cci=5;cci<=mode_c;cci++)
			{
				//while(tp_dev.scan(0)==0);
				if(count_10ms>=10+beep_ks)
				{
					beep_ks=0;
					TIM_Cmd(TIM1, DISABLE);
				}
				if(tp_dev.scan(0)==1)
			{
				if((tp_dev.x[cm]>show[3]*140-20)&&(tp_dev.x[cm]<show[3]*140+115+20)&&(tp_dev.y[cm]>420-20)&&(tp_dev.y[cm]<560-20))
				{
					while(1)
		      {
			      tp_dev.scan(0);
					  if(!((tp_dev.x[cm]>show[3]*140-20)&&(tp_dev.x[cm]<show[3]*140+115+20)&&(tp_dev.y[cm]>420-20)&&(tp_dev.y[cm]<560-20))) break;
				  }
					if(count_2==0)
					{
						count_10ms=0;
						TIM_Cmd(TIM2, ENABLE);
					}
					
					
					TIM_Cmd(TIM1, ENABLE);
					change_fre(1319);
					beep_ks=count_10ms;
					
					
					LCD_Clear(WHITE);
					show[4]=show[3];
					show[3]=show[2];
					show[2]=show[1];
					show[1]=show[0];
					show[0]=jd[diyi+5+count_2];
					
					count_2++;
					if((count_2%7)==0)
					{
						show[1]=show[4];
					}
					
					for(i=0;i<=4;i++)
					{
						if(i==0)
							fill_cla_hui(show[4-i],4-i);
						else
						{
							if(count_2<14)
					      fill_cla(show[4-i],4-i);
							else if((count_2>=14)&&(count_2<28))
								fill_cla_lv(show[4-i],4-i);
							else if((count_2>=28)&&(count_2<=42))
								fill_cla_zong(show[4-i],4-i);
							else
								fill_cla_qian(show[4-i],4-i);
						}
					 //LCD_ShowNum(300,705+17*i,show[4-i],1,16);
					}
					
					//count_2++;
					
					if(count_10ms>=mode_chan*100)
						goto succ_chan;
//						break;
				}
				else
		{
			x_c=tp_dev.x[cm]/119;
			y_c=tp_dev.y[cm]/119;
			TIM_Cmd(TIM1, ENABLE);
			change_fre(400);
			for(ii=0;ii<=4;ii++)
						{
						LCD_Fill(x_c*119,y_c*140,x_c*119+119,y_c*140+140,RED);
						delay_ms(100);
						LCD_Fill(x_c*119,y_c*140,x_c*119+119,y_c*140+140,WHITE);
					  delay_ms(100);
						}
						success=0;
						TIM_Cmd(TIM1, DISABLE);
						goto fail_chan;
		}
			}
			}
			
			succ_chan:		
			LCD_Clear(WHITE);
			
				LCD_ShowNum(400,705,count_2,3,16);
			 // LCD_ShowString(448,705,90,16,16,"ge");
			  LCD_Fill(100,100,300,250,BLUE);
				//LCD_ShowString(100,100,90,16,16,"back");
		while(1)
		{
				
//			if(tp_dev.scan(0)==1)
//			{
			tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
       TIM_Cmd(TIM1, DISABLE);                      //////////////////////////////////////////////////////////////////////////////////
			 goto ks;
		}
//		else
//		{
//			x_c=tp_dev.x[cm]/119;
//			y_c=tp_dev.y[cm]/119;
//			for(ii=0;ii<=4;ii++)
//						{
//						LCD_Fill(x_c*119,y_c*140,x_c*119+119,y_c*140+140,RED);
//						delay_ms(100);
//						LCD_Fill(x_c*119,y_c*140,x_c*119+119,y_c*140+140,WHITE);
//					  delay_ms(100);
//						}
//						success=0;
//						break;
//		}
//	  }
		}
		
		
		fail_chan:
		LCD_Clear(WHITE);
		LCD_ShowString(100,10,90,16,16,"fail");
		LCD_Fill(100,100,300,250,BLUE);
		LCD_ShowString(100,100,90,16,16,"back");
		while(1)
		{
			tp_dev.scan(0);
		if(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)
		{
			while(1)
		  {
			    tp_dev.scan(0);
			  if(!(tp_dev.x[cm]>100&&tp_dev.x[cm]<300&&tp_dev.y[cm]>100&&tp_dev.y[cm]<250)) break;
	     }
       TIM_Cmd(TIM1, DISABLE);                      //////////////////////////////////////////////////////////////////////////////////
			 goto ks;
		}
	}
	
	
	
	
//	LCD_Fill_zhx(0,0,3,700,BLACK);
//	LCD_Fill_zhx(119,0,122,700,BLACK);
//	LCD_Fill_zhx(238,0,241,700,BLACK);
//	LCD_Fill_zhx(357,0,360,700,BLACK);
//	LCD_Fill_zhx(476,0,479,700,BLACK);
	//chushi();
	//TIM_Cmd(TIM4, ENABLE);
	
	
	
	while(1)
	{
		//LCD_ShowNum(220,600,rd,1,16);
		delay_ms(100);

//		}
	}		

}//main




void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //清除TIMx更新中断标志 
			
			count_1++;
			bs=count_2/3+6;
			if(bs>=16)
				bs=16;
			if(mode==0)
			{
			for(i=5;i>=0;i--)
			{
				if(Dian[i][0].Y>=700)
				{
					if((Dian[i][0].an==0)&&(Dian[i][1].an==0)&&(Dian[i][2].an==0)&&(Dian[i][3].an==0))
						si=1;
					else
						si=0;
					if((count_3>=6)&&(si==1))
					{
						si=0;
						sisi=1;
					}
					count_3++;
					LCD_ShowNum(250,710,count_3,3,16);
					srand(time_us);
					rd=rand()%4;
					
					for(j=0;j<=3;j++)
					{
						if(j==rd)
						{
							Dian[i][j].co=1;
						}
						else
						{
							Dian[i][j].co=0;
						}
						Dian[i][j].Y=-140;
						Dian[i][j].an=0;
					}
				}
				for(j=0;j<=3;j++)
				{
					xiangxia_2(i,j,bs);
				}
			}
//			if(tp_dev.scan(0)==1)
//			{
//				if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<700)
//			  {	
//				  ti_min=findmin_y();
//					tt=(tp_dev.y[0]-Dian[ti_min][0].Y)/140;
//					ti=ti_min+tt;
//					if(ti>=6)
//					{
//						ti=ti-6;
//					}
//					tj=tp_dev.x[0]/119;
//					if(Dian[ti][tj].co==1)
//					{
//						Dian[ti][tj].an=1;
//					}
//					else
//					{
//						Dian[ti][tj].an=2;
//						LCD_Fill(100,710,200,770,RED);     //ceshi
//					}
//			  }
//			}
		  for(i=5;i>=0;i--)
			{
				LCD_Fill_zhx(Dian[i][0].X+4,Dian[i][0].Y+136,Dian[i][3].X+119,Dian[i][0].Y+136+3,BLACK);
				for(j=0;j<=3;j++)
				{
					if(Dian[i][j].an==0)
					{
					  xiangxia_2_2(i,j,bs);
					}
				}
			}
		}
			
		
		
			else if(mode==1)
			{
				if(tp_dev.scan(0)==1)
			{
				if(tp_dev.x[0]<lcddev.width&&(tp_dev.y[0]<=660)&&(tp_dev.y[0]<=660))
			  {	
				  for(i=0;i<=4;i++)
					{
						//show[4-i]=show[
					}
			  }
			}
			}
		}
}
