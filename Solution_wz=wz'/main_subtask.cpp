/*
 w(z1) = w(z2) <=> wu1(wf11 - wf10) + 2^n * wf10 = wu2(wf21 - wf20) + 2^n * wf20
 */

 /* делаем перебор этих параметров */

#include <iostream>

inline int f(int &wu1, int &wu2, int &wf11, int &wf10, int &wf21, int &wf20, int &n){
    return wu1*(wf11 - wf10) + (1<<n)*wf10 - wu2*(wf21 - wf20) - (1<<n)*wf20;
}
inline bool checkEquals(int &wf10, int &wf11, int &wf20, int &wf21){
    return wf10 == wf20 && wf11 == wf21;
}
inline bool checkWeight_for_u(int &wu1, int &wu2){
    return (wu1 & 1) == 1 && (wu2 & 1) == 1;
}

inline bool checkWeight_for_w(int &wf10, int &wf11, int &wf20, int &wf21){
    return (((wf10 + wf11) % 2)== 1) && (((wf20 + wf21) % 2) == 1);
}
int main(){
    std::cout << "Starting search..." << std::endl;
    int wu1, wu2, wf11, wf10, wf21, wf20, n,m;

    std::cout<<"Enter n: ";
    std::cin>>n;
    m = 3;    //std::cin>>m;
    int nP = 1<<n;
    int mP = 1<<m;
    std::cout<<"wu1\twu2\twf11\twf10\twf21\twf20\tn="<<n<<"\tm="<<m<<std::endl;
    size_t counter = 0;
    for(wu1=1; wu1 < nP; wu1+=2){
        for(wu2=1; wu2 < nP; wu2+=2){
            for(wf11=0; wf11 < mP; wf11++){
                for(wf10=0; wf10 < mP; wf10++){
                    for(wf21=0; wf21 < mP; wf21++){
                        for(wf20=0; wf20 < mP; wf20++){
                            if(checkWeight_for_w(wf10, wf11, wf20, wf21)==true &&
                               checkWeight_for_u(wu1, wu2)==true && 
                               checkEquals(wf10, wf11, wf20, wf21)==false && 
                               f(wu1, wu2, wf11, wf10, wf21, wf20, n) == 0){
                                counter++;
                                std::cout<<wu1<<"\t"<<wu2<<"\t"<<wf11<<"\t"<<wf10<<"\t"<<wf21<<"\t"<<wf20<<std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout<<"Total solutions found: "<<counter<<std::endl;

    return 0;
}