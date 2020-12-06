void setup() {//aici programul ruleaza o singura data, la inceput
  Serial.begin(9600);//precizam pinii pe care ii vom folosi pe placa Arduino
  pinMode(10,INPUT);
  pinMode(11,OUTPUT);
}

int ok=0,m=0,nr=0;//declaram variabilele m si nr (cu care vom calcula luminozitatea medie) si ok, in functie de care programul va rula sau nu

void loop() {//aici programul ruleaza in bucla, la infinit sau pana il oprim noi
  int vl=analogRead(0), but=digitalRead(10);//declaram variabilele vl (valoare lumina) si but (starea butonului (apasat, neapasat))intregi si le initializam cu valorile citite de la senzor, respectiv buton
  float t=analogRead(1);//declaram variabila t (temperatura) reala si o initializam cu valoarea citita de la senzor

  t=(t*5)/1024;//calculam tensiunea (de la 0 la 5 volti) pe care o da senzorul, direct proportionala cu temperatura masurata de acesta
  t-=0.46;//scadem un coeficient, calculat experimental, din valoarea tensiunii
  t*=100;//inmultim tensiunea din care am scazut coeficientul cu 100 si aflam temperatura exprimata in grade Celsius

  if(but!=0 && ok==0)//calculam variabila ok daca butonul e apasat si ok=0, atunci ok devine 1
  {
    ok=1;
    m=0;//cand ok devine 1 programul incepe sa ruleze, iar m si nr, pe care le vom folosi ca sa calculam media aritmetica a luminozitatii, sunt reinitializate cu 0
    nr=0;
  }
  else if(but!=0 && ok==1)//ca ok sa devina inapoi 0 trebuie sa apasam din nou butonul
    ok=0;

  //daca ok este 1, atunci programul ruleaza; acesta trebuie sa functioneze doar cand vor utilizatorii, ca sa nu fii avertizat sa-ti pui masca atunci cand te intalnesti pe strada cu cineva care locuieste cu tine
  if(ok==1)
  {
    //verifica daca in jurul tau "ceva" a obturat lumina si senzorul a inregistrat o modificare fata de media calculata, iar mai apoi verifica daca temperatura acelui "ceva" e intre 35 si 40 de grade
    //adica afla daca cel care se apropie de tine e om sau nu, altfel avertizandu-te sa-ti pui masca si daca treci pe langa un copac sau o casa
    if(vl<(m/nr)-50 && t<=40 && t>=35)
      digitalWrite(11, HIGH);//daca e om, LED-ul de avertizare se aprinde si te atentioneaza ca trebuie sa-ti pui masca
    else//daca nu a trecut un om pe langa tine, luminozitatea curenta este inregistrata in medie
    {
      m+=vl;//m reprezinta suma tuturor valorilor inregistrate
      nr++;//nr reprezinta numarul de valori inregistrate, m/nr fiind media aritmetica
      
      digitalWrite(11, LOW);//daca nu e om, LED-ul se stinge sau ramane stins
    }
  }
  else
    digitalWrite(11, LOW);//daca programul nu ruleaza, LED-ul se stinge sau ramane stins

  if(nr>=1000)//la fiecare 1000 de valori inregistrate se reinitializeaza numaratoarea si suma tuturor valorilor
  {
    nr=0;
    m=0;
  }
  
  Serial.print(m/nr);//se afiseaza media luminozitatii
  Serial.print(' ');
  Serial.print(vl);//se afiseaza luminozitatea curenta
  Serial.print(' ');
  Serial.print(t);//se afiseaza temperatura obiectului
  Serial.print(' ');
  Serial.println(ok);//se afiseaza variabila ok, in functie de care programul ruleaza sau nu
  delay(50);
