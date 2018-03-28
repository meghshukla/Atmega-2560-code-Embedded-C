static unsigned char pizza_shop[10] = {0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99};     //Elements define what pizza at individual; stop
static unsigned char order_timeline_1[7] = {0x71,0xD8,0xA2,0x5B,0x7C,0x95,0xE5};		      //Order Timeline for pizza type and house
static unsigned char order_track[7] = {0,0,0,0,0,0,0};
static unsigned char order_type[7] = {'r','r','p','p','r','p','p'};
static unsigned int  order_time[7] = {40,100,115,200,250,350,350};
static unsigned char timeline_count = 0;
unsigned char estimate_house[12] ={12,17,23,25,25,23,17,12,13,18};
unsigned char estimate_shop[10] = {21,18,16,14,12,12,14,16,18,21};
void logic (void)
{
	short int k;
	short int i;
	short int l=0;
	short int n=0;
	short int o=0;
	short int j=0;
	unsigned char houseconvert0;
	unsigned char houseconvert1;
	unsigned char houseconvert2;
	unsigned char houseconvert3;
	unsigned char pizzaconvert0;
	unsigned char pizzaconvert1;
	unsigned char pizzaconvert2;
	unsigned char pizza1size;
	unsigned char pizza2size;
	unsigned char pizza1colour;
	unsigned char pizza2colour;
	unsigned int order_process[3] = {0, 0, 0};
	unsigned int order_accepted[2] = {0, 0};
	restart:	for(k=0;k<=7;k++)						//job scheduling. k=1 means corresponding order delivered. 0 indicates not delivered.
	//                k=2 means delivery later time (TIP time elapsed for example)
	{
		if(order_track[k]==0)
		{
			order_process[l]=k;							//passing of order numbers to different array, which holds orders
			l++;										//that need to be in consideration for delivery management
			if(l==3)									//at a time three orders considered
			break;										//THESE NUMBERS ARE USED TO DETERMINE HOUSE and PIZZA
		}
	}
	if(order_process[0]==0 && order_process[1]==0 && order_process[0]==0)      //0 all three places indicates order_track array as either 1 or 2.
	{																		   //initially, order track all 0
		for(k=0;k<=7;k++)
		{																	   //as robot keeps delivering, order_track changes value to 1 or 2
			if(order_track[k]==2)						                       // 1 indicate delivered and 2 means delivery later time
			{																   //if order track non zero, no new orders pending, we can deliver earlier orders
				order_process[l]=k;							                   //(denoted by 2) without affecting tip
				l++;
				if(l==3)
				break;
			}
		}
	}
	houseconvert0=order_timeline_1[order_process[0]] & 0x0F;
	houseconvert1=order_timeline_1[order_process[1]] & 0x0F;     //decode house from timeline value
	houseconvert2=order_timeline_1[order_process[2]] & 0x0F;	 //please refer to start of code for how order
	houseconvert3=order_timeline_1[order_process[2]+1] & 0x0F;   //stored in timeline array. Order process used to access various
	pizzaconvert0=order_timeline_1[order_process[0]] & 0xF0;	 //timeline orders
	pizzaconvert1=order_timeline_1[order_process[1]] & 0xF0;
	pizzaconvert2=order_timeline_1[order_process[2]] & 0xF0;
	
	//ORDER DECIDING PHASE. WHICH ORDERS RoBoT should accept based on type of pizza
	if(order_type[order_process[0]]=='r' && order_type[order_process[1]]=='r')
	{
		if((houseconvert0<0x04 && houseconvert1<0x04) || (((houseconvert0>0x04)&&(houseconvert0<=0x08)) && ((houseconvert1>0x04)&&(houseconvert1<=0x08))) || abs(houseconvert0-houseconvert1)== (1||7))
		//If order house lies on same side of flex or adjacent accross the sides. Eg H4 and H5
		{
			if(order_timeline_1[order_process[1]]-order_timeline_1[order_process[0]]<=10)  //difference between two orders is 10s
			{
				for(n=0;n<=9;n++)
				{
					if(pizza_shop[n]==pizzaconvert0)
					break;
				}
				for(o=0;o<=9;o++)
				{
					if(pizza_shop[o]==pizzaconvert1)
					break;
				}
				if(n!=10 && o!=10)    //if difference is less than 10 and pizza locations known
				{
					attempt_two_delivery=1;
					order_accepted[0]=order_process[0];
					order_accepted[1]=order_process[1];
					order_track[order_process[0]]=1;
					order_track[order_process[1]]=1;
				}
			}
		}
		else         //attempt single delivery if above conditions not specified. For TWO REGULAR
		{
			attempt_two_delivery=0;
			order_accepted[0]=99;  //99 indicating only one order
			order_accepted[1]=order_process[0];
			order_track[order_process[0]]=1;
		}
	}
	else if(order_type[order_process[0]]=='r' && order_type[order_process[1]]=='r' && order_type[order_process[2]]=='p')
	{
		if((houseconvert0<0x04 && houseconvert1<0x04) || (((houseconvert0>0x04)&&(houseconvert0<=0x08)) && ((houseconvert1>0x04)&&(houseconvert1<=0x08))) || abs(houseconvert0-houseconvert1)== (1||7))
		{
			if(order_timeline_1[order_process[1]]-order_timeline_1[order_process[0]]<=10)
			{
				for(n=0;n<=9;n++)
				{
					if(pizza_shop[n]==pizzaconvert0)
					break;
				}
				for(o=0;o<=9;o++)
				{
					if(pizza_shop[o]==pizzaconvert1)
					break;
				}
				if(n!=10 && o!=10)
				{
					attempt_two_delivery=1;
					order_accepted[0]=order_process[0];
					order_accepted[1]=order_process[1];
					order_track[order_process[0]]=1;
					order_track[order_process[1]]=1;
				}
			}
		}
		//Special case, If the third order taken into consideration is preorder and time window lies somehwere near delivery time
		//of regular pizza.
		else if((order_time[order_process[2]]<=order_time[order_process[0]]+55) && (houseconvert3!=houseconvert2))
		{
			for(n=0;n<=9;n++)
			{
				if(pizza_shop[n]==pizzaconvert0)
				break;
			}
			for(o=0;o<=9;o++)
			{
				if(pizza_shop[o]==pizzaconvert2)
				break;
			}
			if(n!=10 && o!=10)
			{
				attempt_two_delivery=1;
				order_accepted[0]=order_process[0];
				order_accepted[1]=order_process[2];
				order_track[order_process[0]]=1;
				order_track[order_process[2]]=1;
			}
			
		}
		else
		{
			attempt_two_delivery=0;
			order_accepted[0]=99;
			order_accepted[1]=order_process[0];
			order_track[order_process[0]]=1;
		}
	}
	else if(order_type[order_process[0]]=='r' && order_type[order_process[1]]=='p')
	{
		if((order_time[order_process[1]]<=order_time[order_process[0]]+60) && (houseconvert1!=houseconvert2))
		//the second condition in loop checks for if same house orders two pizzas. if it does, then preorder pizza is dropped
		//from consideration. better deliver the two pizzas in one go only rather than separate it.
		{
			for(n=0;n<=9;n++)
			{
				if(pizza_shop[n]==pizzaconvert0)
				break;
			}
			for(o=0;o<=9;o++)
			{
				if(pizza_shop[o]==pizzaconvert1)
				break;
			}
			if(n!=10 && o!=10)
			{
				attempt_two_delivery=1;
				order_accepted[0]=order_process[0];
				order_accepted[1]=order_process[1];
				order_track[order_process[0]]=1;
				order_track[order_process[1]]=1;
			}
			
		}
		else
		{
			attempt_two_delivery=0;
			order_accepted[0]=99;
			order_accepted[1]=order_process[0];
			order_track[order_process[0]]=1;
		}
	}
	else if(order_type[order_process[0]]=='p' && order_type[order_process[1]]=='r')
	//advantage of preorder lies on double time window. Halfway past the time window, it acts as regular order only.
	//hence similar condition to TWO REGULAR pizza orders.
	{
		if((houseconvert0<0x04 && houseconvert1<0x04) || (((houseconvert0>0x04)&&(houseconvert0<=0x08)) && ((houseconvert1>0x04)&&(houseconvert1<=0x08))) || abs(houseconvert0-houseconvert1)== (1||7))
		{
			if(order_timeline_1[order_process[1]]-order_timeline_1[order_process[0]]<=10)
			{
				for(n=0;n<=9;n++)
				{
					if(pizza_shop[n]==pizzaconvert0)
					break;
				}
				for(o=0;o<=9;o++)
				{
					if(pizza_shop[o]==pizzaconvert1)
					break;
				}
				if(n!=10 && o!=10)
				{
					attempt_two_delivery=1;
					order_accepted[0]=order_process[0];
					order_accepted[1]=order_process[1];
					order_track[order_process[0]]=1;
					order_track[order_process[1]]=1;
				}
			}
		}
		else
		{
			attempt_two_delivery=0;
			order_accepted[0]=99;
			order_accepted[1]=order_process[0];
			order_track[order_process[0]]=1;
		}
	}
	//if both preorders same house, we attempt delivery for both
	else if(order_type[order_process[0]]=='p' && order_type[order_process[1]]=='p')
	{
		if(houseconvert0==houseconvert1)
		{
			attempt_two_delivery=1;
			order_accepted[0]=order_process[0];
			order_accepted[1]=order_process[1];
			order_track[order_process[0]]=1;
			order_track[order_process[1]]=1;
		}
		//if different house, condition identical to ONE REGULAR AND PREORDER. Explained earlier
		else if((order_time[order_process[1]]<=order_time[order_process[0]]+55) && (houseconvert1!=houseconvert2))
		{
			for(n=0;n<=9;n++)
			{
				if(pizza_shop[n]==pizzaconvert0)
				break;
			}
			for(o=0;o<=9;o++)
			{
				if(pizza_shop[o]==pizzaconvert1)
				break;
			}
			if(n!=10 && o!=10)
			{
				attempt_two_delivery=1;
				order_accepted[0]=order_process[0];
				order_accepted[1]=order_process[1];
				order_track[order_process[0]]=1;
				order_track[order_process[1]]=1;
			}
			
		}
		else
		{
			attempt_two_delivery=0;
			order_accepted[0]=99;
			order_accepted[1]=order_process[0];
			order_track[order_process[0]]=1;
		}
	}
	
	//delivery not able to make it on time CONDITION
	if(time_ssd==order_time[order_process[0]]+20)
	{
		order_track[order_accepted[0]]=2;
		goto restart;
	}
	
	//PIZZA IDENTIFICATION PHASE
	if(attempt_two_delivery==0)          //only one delivery
	{									 //try to scan till particular time after order placed. If block not found, abort order and deliver at end.
		
		for(n=0;n<=9;n++)
		{
			if(pizza_shop[n]==(order_timeline_1[order_accepted[1]]&0xF0))
			break;
		}
		if(n==10)      //pizza location not known
		{
			loop1:  // SCAN shop while time is still enough to deliver pizza. If IF condition not satisfied, it implies robot didnt find pizza on time.
			if(order_time[order_accepted[0]]+40>=time_ssd+estimate_house[(int)(order_timeline_1[order_accepted[0]] & 0x0F)])
			{
				
				for(i=0;i<=9;i++)
				{
					if(pizza_shop[i]==0x99)
					break;
				}
				j=i;
				for(;j<=9;j++)
				{
					if(pizza_shop[j]==0x99)
					break;
				}
				move_shop('s',i);
				sense(i);
				if(pizza_shop[i]!=(order_timeline_1[order_accepted[1]]&0xF0))   //pizza not found
				{
					move_shop(i,j);
					sense(j);
					if(pizza_shop[j]!=(order_timeline_1[order_accepted[1]]&0xF0))
					{
						goto loop1;				//loop if pizza not found.
					}
				}
			}
			else      //if condition failed, time limit exceeded, abort order and mark it for later delivery (indicated by 2)
			{
				order_track[order_accepted[1]]=2;
				goto restart;
			}
		}
		//pizza found if it has not gone to loop1
		//scan while estimated delivery time is 35 seconds + order time. In previous IF after loop1:, we considered 40s as
		//if pizza found at 40, it can still deliver on time. However, scan pizza shop till 35s over order time  after
		//locating pizza as robot needs time to come back to pizza counter. 5s time considered.
		while(order_time[order_accepted[0]]+35>=time_ssd+estimate_house[(int)(order_timeline_1[order_accepted[0]] & 0x0F)])        //scan shop till it is time to deliver pizza
		{
			for(i=0;i<=9;i++)
			{
				if(pizza_shop[i]==0x99)
				break;
			}
			j=i;
			for(;j<=9;j++)   //identify pizza shop where pizza still not identified
			{
				if(pizza_shop[j]==0x99)
				break;
			}
			if(j<=9)
			{
				move_shop(i,j);
				sense(j);
			}
		}
		//exit while loop means time to deliver order (only one pizza carry condition specified by attempt_two_delivery variable
	}
	if(attempt_two_delivery==1)      //order accepted[0] indicates it needs to be delivered first. order accepted [1] indicates second delivery.
	{
		for(n=0;n<=9;n++)
		{
			if(pizza_shop[n]==(order_timeline_1[order_accepted[0]]&0xF0))
			break;
		}
		if(n==10)      //pizza location not known
		{
			loop2:  // SCAN shop while time is still enough to deliver pizza. If IF condition not satisfied, it implies robot didnt find pizza on time.
			if(order_time[order_accepted[0]]+40>=time_ssd+estimate_house[(int)(order_timeline_1[order_accepted[0]] & 0x0F)])
			{
				
				for(i=0;i<=9;i++)
				{
					if(pizza_shop[i]==0x99)
					break;
				}
				j=i;
				for(;j<=9;j++)
				{
					if(pizza_shop[j]==0x99)
					break;
				}
				move_shop('s',i);
				sense(i);
				if(pizza_shop[i]!=(order_timeline_1[order_accepted[0]]&0xF0))   //pizza not found
				{
					move_shop(i,j);
					sense(j);
					if(pizza_shop[j]!=(order_timeline_1[order_accepted[0]]&0xF0))
					{
						goto loop2;				//loop if pizza not found.
					}
				}
			}
			else      //if condition failed, time limit exceeded, abort order and mark it for later delivery.
			{
				order_track[order_accepted[0]]=2;
				goto restart;
			}
		} //for attempt_two_delivery==1, condition same as for 0. First priority pizza needs to be found. Indicated
		//by ORDER_ACCEPTED[0]. since that delivery window gets over earlier.
		
		//We found first pizza. now similar sequence for second pizza.
		for(n=0;n<=9;n++)
		{
			if(pizza_shop[n]==(order_timeline_1[order_accepted[1]]&0xF0))
			break;
		}
		if(n==10)      //pizza location not known
		{
			loop3:  // SCAN shop while time is still enough to deliver pizza. If IF condition not satisfied, it implies robot didnt find pizza on time.
			if(order_time[order_accepted[1]]+40>=time_ssd+estimate_house[(int)(order_timeline_1[order_accepted[1]] & 0x0F)])
			{
				
				for(i=0;i<=9;i++)
				{
					if(pizza_shop[i]==0x99)
					break;
				}
				j=i;
				for(;j<=9;j++)
				{
					if(pizza_shop[j]==0x99)
					break;
				}
				move_shop('s',i);
				sense(i);
				if(pizza_shop[i]!=(order_timeline_1[order_accepted[1]]&0xF0))   //pizza not found
				{
					move_shop(i,j);
					sense(j);
					if(pizza_shop[j]!=(order_timeline_1[order_accepted[1]]&0xF0))
					{
						goto loop3;				//loop if pizza not found.
					}
				}
			}
			else      //if condition failed, time limit exceeded, abort order and mark it for later delivery.
			{
				order_track[order_accepted[1]]=2;
				attempt_two_delivery=0;       //second priority pizza not found, abort it, single delivery only
				order_accepted[1]=order_accepted[0];
				order_accepted[0]=99;               // for single delivery only order_accepted[1] used
			}
		}
	}
	
	//PIZZA PICKING PHASE
	if(attempt_two_delivery==0)
	{
		for(n=0;n<=9;n++)
		{
			if(pizza_shop[n]==(order_timeline_1[order_accepted[1]]&0xF0))
			break;
		}
		return_shop(j+1);  //robot is at Jth location during search. J+1 because return_shop uses 1 to 10 as reference and not 0 to 9
		move_shop('s',n+1);
		pizza1size=(order_timeline_1[order_accepted[1]] & 0x30);
		pizza1colour=(order_timeline_1[order_accepted[1]] & 0xC0);
		if(pizza1size==0x01)           //descriptions given at top of code
		{
			pick_block('s',1);
		}
		if(pizza1size==0x02)
		{
			pick_block('m',1);
		}
		if(pizza1size==0x03)
		{
			pick_block('l',1);
		}
		led_color(pizza1colour);
		return_shop(n+1);
	}
	else
	{
		for(n=0;n<=9;n++) //pick priority 2 pizza
		{
			if(pizza_shop[n]==(order_timeline_1[order_accepted[1]]&0xF0))
			break;
		}
		return_shop(j+1);  //robot is at Jth location during search. J+1 because return_shop uses 1 to 10 as reference and not 0 to 9
		move_shop('s',n+1);
		pizza1size=(order_timeline_1[order_accepted[1]] & 0x30);
		pizza1colour=(order_timeline_1[order_accepted[1]] & 0xC0);
		if(pizza1size==0x01)           //descriptions given at top of code
		{
			pick_block('s',1);
		}
		if(pizza1size==0x02)
		{
			pick_block('m',1);
		}
		if(pizza1size==0x03)
		{
			pick_block('l',1);
		}
		led_color(pizza1colour);
		
		//pick priority1 pizza
		for(o=0;o<=9;n++)
		{
			if(pizza_shop[o]==(order_timeline_1[order_accepted[0]&0xF0]))
			break;
		}
		move_shop(n+1,o+1);  //move_shop() can directly go from one node to next without going to pizza counter every time
		pizza2size=(order_timeline_1[order_accepted[0]] & 0x30);
		pizza2colour=(order_timeline_1[order_accepted[0]] & 0xC0);
		
		if(pizza2size==0x01)           //descriptions given at top of code
		{
			pick_block('s',0);
		}
		if(pizza2size==0x02)
		{
			pick_block('m',0);
		}
		if(pizza2size==0x03)
		{
			pick_block('l',0);
		}
		led_color(pizza2colour);
		return_shop(o+1);
	}
	
	//PIZZA DEPOSITION PHASE
	if(attempt_two_delivery==0)
	{
		move_house('s',(order_timeline_1[order_accepted[1]]&0x0F));  //masking size and colour
		if(pizza1size==0x01)           //descriptions given at top of code
		{
			drop_block('s',1);
		}
		if(pizza1size==0x02)
		{
			drop_block('m',1);
		}
		if(pizza1size==0x03)
		{
			drop_block('l',1);
		}
		ssd_freeze=1;     //sound buzzer and freeze display
		led_color(0);
		for (n=0;n<=7;n++)
		{
			if(order_track[n]==0||2)
			break;
		}
		if (n!=8)      //deliveries still remaining
		return_house(order_timeline_1[order_accepted[1]]&0x0F);
		else
		{
			buzzer_on();
			_delay_ms(5000);
			while(1);         //end of RUN
			
		}
	}
	if(attempt_two_delivery==1)
	{
		//deliver first block
		move_house('s',(order_timeline_1[order_accepted[0]]&0x0F));  //masking size and colour
		if(pizza2size==0x01)           //descriptions given at top of code
		{
			drop_block('s',1);
		}
		if(pizza2size==0x02)
		{
			drop_block('m',1);
		}
		if(pizza2size==0x03)
		{
			drop_block('l',1);
		}
		ssd_freeze=1;     //sound buzzer and freeze display
		led_color(pizza2colour);
		move_house((order_timeline_1[order_accepted[0]]&0x0F),(order_timeline_1[order_accepted[1]]&0x0F));
		if(pizza1size==0x01)           //descriptions given at top of code
		{
			drop_block('s',1);
		}
		if(pizza1size==0x02)
		{
			drop_block('m',1);
		}
		if(pizza1size==0x03)
		{
			drop_block('l',1);
		}
		ssd_freeze=1;
		led_color(0);
		for (n=0;n<=7;n++)
		{
			if(order_track[n]==0||2)
			break;
		}
		if (n!=8)      //deliveries still remaining
		return_house(order_timeline_1[order_accepted[1]]&0x0F);
		else
		{
			buzzer_on();
			_delay_ms(5000);
			while(1);         //end of RUN
		}
		goto restart;   //process repeats all over again!
	}
}