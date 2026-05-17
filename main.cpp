#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include "produse.h"
#include "bauturi.h"
#include "pizza.h"
#include "desert.h"
#include "client.h"
#include "comanda.h"
#include "exceptii.h"
#include "istoric.h"
#include "factory.h"

int main(){
comanda meniu;
IstoricProxy<std::string> istoricAzi;
IstoricProxy<float> venitAzi;

//citire
try{
    std::ifstream  fin_pizza("pizza_in.txt"), fin_bauturi("bauturi_in.txt"), fin_desert("desert_in.txt");
    if(fin_pizza){
        int n;
        fin_pizza>>n;
        for(int i = 0; i<n; i++){
            auto p = ProdusFactory::creeazaProdus(1);
            fin_pizza >> *p;
            meniu.push_back(p);}}

    if(fin_bauturi){
        int n;
        fin_bauturi>>n;
        for(int i=0; i<n; i++){
            auto b = ProdusFactory::creeazaProdus(2);
            fin_bauturi >> *b;
            meniu.push_back(b);}}

    if(fin_desert){
        int n;
        fin_desert>>n;
        for(int i=0; i<n; i++){
            auto d = ProdusFactory::creeazaProdus(3);
            fin_desert >> *d;
            meniu.push_back(d);}}
} catch(const exceptii& e){std::cout<<"eroare"<<e.what()<<"\n Verificati fisierele de intrare.\n"; return 0;}

std::cout<<"Buna ziua, anangajat al Pizzeria Proiect!\n";
int aux=0;
while(aux !=5){
    try{
        std::cout<<"\nAlegeti obtiunea dorita:\n";
        std::cout<<"1) Afisat meniul pe azi\n";
        std::cout<<"2) Editatii meniul pe azi\n";
        std::cout<<"3) Introduceti o comanda noua\n";
        std::cout<<"4)Istoric\n";
        std::cout<<"5) Inchidere\n";
        std::cout<<"Obtiunea = ";
        if(!(std::cin>>aux)) throw intInvalid();


        switch(aux){
        case 1:{
            std::cout<<"========MENIUL DE AZI========\n"<<meniu;
            break;}

        case 2:{
            std::cout<<"Ce doriti sa faceti?\na)Sa adaug un produs.\nb)Sa sterg un produs\nObtiunea=";
            char edit;
            std::cin>>edit;

            switch(edit){
                case 'a':{
                    int tip, continua=1;
                    do{std::cout<<"Tipul produsului de introdus(1-pizza; 2-bautura; 3=desert) = ";
                        if(!(std::cin>>tip)) throw intInvalid();
                        auto p = ProdusFactory::creeazaProdus(tip);
                        std::cin >> *p;
                        meniu.push_back(p);
                        istoricAzi.arhiveazaActiunea("S-a adaugat produsul: " + p->getNume());
                        std::cout<<"Mai vreti sa adaugati ceva? (1=da, 0=nu)\n Alegere = ";
                        if(!(std::cin>>continua)) throw intInvalid();
                    }while(continua !=0);
                break;}

                case 'b':{
                    int continua = 1;
                    do{ if (meniu.empty()){std::cout << "Meniul este gol, nu am ce sterge!\n"; break; }
						std::cout<<meniu<<"Indexul produsului pe care vreti sa il stergeti(unul singur): ";

						int index;
                        if(!(std::cin>>index)) throw intInvalid();
                        std::string numeProdusSters = meniu[index-1]->getNume();
                        meniu.stergeProdus(index-1);
                        istoricAzi.arhiveazaActiunea("S-a sters din meniu produsul: " + numeProdusSters);
                        std::cout<<"Mai vreti sa stergeti ceva? (1=da, 0=nu)\n Alegere = ";
                        if(!(std::cin>>continua)) throw intInvalid();
                    }while(continua ==!0);
                break;}
            default: std::cout<<"optinue invalida!\n"; break;}
            break;}

        case 3:{
            std::cout<<"---------Comanda noua---------";
            std::cout<<"Introduceti datele clientului:\n";
            std::string numeC, adresaC, telefonC;
            std::cout << "Nume: "; std::cin.get(); std::getline(std::cin, numeC);
            std::cout << "Adresa: "; std::getline(std::cin, adresaC);
            std::cout << "Telefon: "; std::cin >> telefonC;

            client c(numeC, adresaC, telefonC);
            comanda com(c);
            int alegere=-1, varsta = 0;
            while(alegere != 0){
                try{
                    std::cout<<"\n1.Adaga produse\n2.Sterge produse\n3.Afiseaza comanda si totalul\n0.finalizeaza comanda\nAlegere = ";
                    if(!(std::cin>>alegere)) throw intInvalid();

                    if(alegere ==1){
                        int indexAdauga=-1;
                        std::cout<<"--------MENIU---------\n"<<meniu<<"\nIntroduceti indexul produsului pe care il vrea clientul. Apasati 0 pentru finalizare.";
                        while(indexAdauga!=0 ){
                            if(!(std::cin>>indexAdauga)) throw intInvalid();
                            if(indexAdauga>0 && indexAdauga <= (int)meniu.size()){
                                try{com.adaugaProdus(meniu[indexAdauga-1]);
                                    std::cout<<">>adaugat:"<<meniu[indexAdauga-1]->getNume()<<"\n";
                                    istoricAzi.arhiveazaActiunea("Clientul" +c.getNume()+ "a adaugat la comanda produsul: " + meniu[indexAdauga-1]->getNume());
                                }catch(const varstaAlcool& e){
                                    if(varsta == 2){ std::cout<<"Refuz, client minor! Produsul nu a fost adaugat\n";}
                                    else{
                                        std::cin.clear();
                                        std::cin.ignore(1000, '\n');
                                        std::cout<<"\n!!"<<e.what()<<"!!!\nintrebati clientul daca are peste 18 ani, 1-da, are; 2-nu, nu are; \n>>>";
                                        if(!(std::cin>>varsta)) throw intInvalid();

                                        if(varsta == 1){
                                            com.adaugaProdus(meniu[indexAdauga - 1], true);
                                            std::cout <<">>adaugat:"<<meniu[indexAdauga-1]->getNume()<<"\nSa informati cuierul sa ceara buletin.\n";}
                                        else{varsta = 2; std::cout<<"Refuz: Client minor! Din acest moment, bauturile alcoolice sunt blocate pentru aceasta comanda.\n";}
                                    }
                                }
                                catch(const exceptii& e){
                                    std::cout << "\nEROARE " << e.what() << "\nVa rugam sa reincercati.\n";
                                    std::cin.clear();
                                    std::cin.ignore(1000, '\n');}
                            }else if(indexAdauga!=0){std::cout<<"intex invalid!\n";}
                        }
                    }

                    else if (alegere == 2){
                        int indexStergere = -1;
                        if (com.empty()){std::cout << "Comanda este goal, nu am ce sterge!\n";}
                        while (indexStergere != 0 && !(com.empty())) {
                            std::cout << "-------COMANDA--------\n" << com<< "Index-ul din comanda a produsului sters (unul singur) =  ";
                            if(!(std::cin >> indexStergere)) throw intInvalid();
                            std::string numeProdusSters = com[indexStergere - 1]->getNume();
                            com.stergeProdus(indexStergere - 1);
                            istoricAzi.arhiveazaActiunea("Clientul" +c.getNume()+ "a sters din comanda produsul: " + numeProdusSters);
                        }
                    }

                    else if(alegere == 3) {std::cout<<"------------COMANDA----------\n"<<com<<"\nTimp preparare = "<<com.timpPreparareTotal()<<" minute.\n";}
                    else if(!(alegere==0 || alegere == 1 || alegere==2 || alegere==3)) std::cout<<"Invali! Introduceti un numar intre 0 si 3";

            }catch (const exceptii& e){
                std::cout << "\nEROARE " << e.what() << "\nVa rugam sa reincercati.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');}}

            std::cout<<"(de citit)'Totalul vostru este de "<<com.pretTotal()<<" lei. Cash sau card?\n";
            std::cout<<"Comanda finalizata!!\nTimp preparare = "<<com.timpPreparareTotal()<<" minute.\n";
            std::cout<<"\nVa multumim!'\n";
            istoricAzi.arhiveazaActiunea("Clientul" +c.getNume()+ "a finalizat comanda, si a platit " +std::to_string(com.pretTotal())+"lei");
            venitAzi.arhiveazaActiunea(com.pretTotal());
        break;}

        case 4: {
            int obt=0;
            std::string p;
            try {
                std::cout<<"Ce doriti sa faceti? \n1)Vizualizat ultima actiune\n2)Vizualizat intregul istoric de actiuni de azi\n3)Stergere istoric actiuni de azi\n";
                std::cout<<"4)Vizualizat veniturile totale de azi\n5)Stergeti istoricul veniturilor de azi\n";
                std::cout<<"ATENTIE: Obtiunea 2 si 3, din motive de securitate, se pot executa doar de manager!\n Obtiune = ";
                if(!(std::cin>>obt)) throw intInvalid();
                switch(obt){
                case 1:{
                    std::cout<<"Ultima actiune a fost:\n";
                    istoricAzi.afiseazaUltima();
                    break;}

                case 2:{
                    std::cout<<"Introducetii parola:";
                    std::cin>>p;
                    istoricAzi.afiseazaIntregIstoric(p);
                    break;}

                case 3:{
                    std::cout<<"Introducetii parola:";
                    std::cin>>p;
                    istoricAzi.resetareIstoric( p);
                    break;}

                case 4:{
                    std::cout<<"Introducetii parola:";
                    std::cin>>p;
                    venitAzi.afiseazaIntregIstoric(p);
                    break;}

                case 5:{
                    std::cout<<"Introducetii parola:";
                    std::cin>>p;
                    venitAzi.resetareIstoric(p);
                    break;}

                default: std::cout<<"Optiune invalida!";
                }
            } catch (const exceptii& e) {std::cout << "\n[ACCES REFUZAT]: " << e.what() << "\n";}
        break;}

        case 5:{
            std::cout<<"\nInchidere......\nFelicitari! Ati procesat "<<comanda::getNrComenzi()<<" de comenzi si ati ajutat "<<client::getNrClienti()<<" clienti sa se bucure de cea mai buna pizza din europa de est!\nLa revedere si te vedem si maine!";
            break;}

        default:{
            std::cout<<"Va rog sa alegeti o obtiune intre 1 si 5";
            break;}
        }
    }
    catch (const exceptii& e) {
        std::cout << e.what() << "\nVa rugam sa reincercati.\n";
        std::cin.clear();
        std::cin.ignore(1000, '\n');}
}}





