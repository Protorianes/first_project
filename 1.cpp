#include <iostream>
#include <map>
using namespace std;
struct Order{
	int ID;
	char side;
	int Qty;
	double Price;
	//string Instrument;
};

Order ParsOrder(string s){
	int sID=0,sQty=0;
	double sPrice=0.0,dr=0.0;
	char sside='N';
	unsigned int i=2;
	
	while(s[i]!=','){
		sID*=10;
		sID+=(s[i]-48);
		i++;
	}
	i++;
	sside=s[i];
	i+=2;
	while(s[i]!=','){
		sQty*=10;
		sQty+=(s[i]-48);
		i++;
	}
	i++;
	while(s[i]!='.'){
		sPrice*=10;
		sPrice+=(s[i]-48)+0.0;
		i++;
	}
	i++;
	if(i<s.length()){
		dr=(s[i]-48)/10.0;
		sPrice+=dr;
	}
	i++;
	if(i<s.length()){
		dr=(s[i]-48)/100.0;
		sPrice+=dr;
	}
	return{sID,sside,sQty,sPrice};
}


int main(){
	Order F,NewOrder,tempOrder;
	map <pair<double,int>, Order> OrderS;// все заявки-покупки ключ -- (стоимость,ID)
	map <pair<double,int>, Order> OrderB;// все заявки-продажи ключ -- (стоимость,ID)
	
	map <pair<double,int>, Order> :: iterator minQ;// минималная стоимость (стоимость,ID)
	map <pair<double,int>, Order> :: iterator maxQ;// максимальная стоимость (стоимость,ID)
	map <int, Order> OrdersID;// все заявки ключ -- ID
	int  del,TID;
	int Buff[22];
	string input;
	getline(cin, input);
	TID=0;
	while(input!="exit"){
		if(input[0]=='O'){
			NewOrder=ParsOrder(input);//заявка
			if(NewOrder.side=='B'){//хочет купить
				if(OrdersID.size()>0 && OrderS.size()>0){
					minQ = OrderS.begin();//минимальная продажа
					tempOrder = (minQ->second);//заявка с минимальной продажей и ID
					while(tempOrder.Price<=NewOrder.Price){
						if(NewOrder.Qty<=tempOrder.Qty){//получилось всё купить
							if(NewOrder.Qty==tempOrder.Qty){
								TID++;
								cout<<"T,"<<TID<<",S,"<<tempOrder.ID<<","<<NewOrder.ID<<","<<NewOrder.Qty<<","<<tempOrder.Price<<endl;
								OrderS.erase (minQ);
								NewOrder.Qty=0;
								break;
							}
							else{
								TID++;
								cout<<"T,"<<TID<<",S,"<<tempOrder.ID<<","<<NewOrder.ID<<","<<NewOrder.Qty<<","<<tempOrder.Price<<endl;
								OrderS.erase (minQ);
								tempOrder.Qty-=NewOrder.Qty;
								OrderS.insert (pair <pair <double,int> ,Order> (make_pair(tempOrder.Price,tempOrder.ID),tempOrder));
								NewOrder.Qty=0;
								break;
							}
						}
						else{
							TID++;
							cout<<"T,"<<TID<<",S,"<<tempOrder.ID<<","<<NewOrder.ID<<","<<tempOrder.Qty<<","<<tempOrder.Price<<endl;		
							NewOrder.Qty-=tempOrder.Qty;
							OrderS.erase (minQ);
							OrdersID.erase (tempOrder.ID);
							if(OrderS.size()>0){
								minQ = OrderS.begin();//обработку NULL доделать
								tempOrder = (minQ->second);//обработку NULL доделать					
							}
							else{
								//break;//вроде норм
								tempOrder=NewOrder;
								tempOrder.Price+=1.0;
							}
						}
					}
				}
				if(NewOrder.Qty!=0){
					OrderB.insert (pair <pair <double,int> ,Order> (make_pair(NewOrder.Price,NewOrder.ID),NewOrder));
					OrdersID.insert (pair <int,Order> (make_pair(NewOrder.ID,NewOrder)));
				}
			}
			if(NewOrder.side=='S'){//хочет продать
				if(OrdersID.size()>0 && OrderB.size()>0){
					maxQ = prev(OrderB.end());//максимальное желание купить
					tempOrder = (maxQ->second);//заявка с максимальной покупкой и ID					
					while(tempOrder.Price>=NewOrder.Price){//покупка дороже продажи						
						if(NewOrder.Qty<=tempOrder.Qty){//получилось всё продать
							if(NewOrder.Qty==tempOrder.Qty){
								TID++;
								cout<<"T,"<<TID<<",B,"<<tempOrder.ID<<","<<NewOrder.ID<<","<<NewOrder.Qty<<","<<tempOrder.Price<<endl;	
								OrderB.erase (maxQ);
								NewOrder.Qty=0;
								break;
							}
							else{
								TID++;
								cout<<"T,"<<TID<<",B,"<<tempOrder.ID<<","<<NewOrder.ID<<","<<NewOrder.Qty<<","<<tempOrder.Price<<endl;
								OrderB.erase (maxQ);
								tempOrder.Qty-=NewOrder.Qty;
								OrderB.insert (pair <pair <double,int> ,Order> (make_pair(tempOrder.Price,tempOrder.ID),tempOrder));
								NewOrder.Qty=0;
								break;
							}
						}
						else{
							TID++;
							cout<<"T,"<<TID<<",B,"<<tempOrder.ID<<","<<NewOrder.ID<<","<<tempOrder.Qty<<","<<tempOrder.Price<<endl;						
							NewOrder.Qty-=tempOrder.Qty;
							OrderB.erase (maxQ);
							OrdersID.erase (tempOrder.ID);
							if(OrderB.size()>0){
								maxQ = prev(OrderB.end());//обработку NULL доделать
								tempOrder = (maxQ->second);//обработку NULL доделать							
							}
							else{
								//break;//вроде норм
								tempOrder=NewOrder;
								tempOrder.Price-=1.0;
							}
						}
					}
				}
				if(NewOrder.Qty!=0){
					OrderS.insert (pair <pair <double,int> ,Order> (make_pair(NewOrder.Price,NewOrder.ID),NewOrder));
					OrdersID.insert (pair <int,Order> (make_pair(NewOrder.ID,NewOrder)));					
				}
			}
		}
		if(input[0]=='C'){		//удаление сделано(вроде)
			for(unsigned int i=2; i<input.length();i++){
				Buff[i-2]=input[i]-48;
			}	
			del=0;//ID удаляемого
			for(unsigned int i=0; i<input.length()-2;i++){
				del = del*10+Buff[i];
			}
			if(OrdersID.count(del)==1){
				cout<<"X,"<<del<<endl;
				F = OrdersID[del];
				OrdersID.erase (del);
				if(F.side=='S'){
					OrderS.erase(make_pair(F.Price,del));	
				}
				if(F.side=='B'){
					OrderB.erase(make_pair(F.Price,del));
				}
			}
		}
		getline(cin, input);
	}
}
