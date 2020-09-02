// 7 parçalý LED gösterge için veri görüntüleme fonksiyonlarý

int8 desen[10]={0x14, 0xD7, 0x4C, 0x45, 0x87, 0x25, 0x24, 0x57, 0x04, 0x05}; // 7 parçalý göstergede rakamlarý vermek için gerekli çýkýþ bit desenleri.
int8 basamaklar[5];
BOOLEAN mhz_artma=FALSE;

void led_goster(void)
   {
      static int8 sayac=4;
      
      output_d(0xFF);
      output_c((0x10<<(sayac-1))^0xF0);
      output_d(basamaklar[sayac]);
      if(sayac!=0x01) sayac--; 
      else sayac=4;
   }

void donustur(int32 veri)
   {
      int8 sayac;
      
      if(veri<100000)
         {
            for(sayac=0;sayac<5;sayac++)
               {
                  basamaklar[sayac]=desen[veri%10];
                  veri/=10;
               }
            sayac=4;
            while((basamaklar[sayac]==0x14)&& sayac>1)
               {
                  basamaklar[sayac]=0xFF;
                  sayac--;
               }
            basamaklar[2]=basamaklar[2] & 0xFB;
            if(basamaklar[0]==0x25) basamaklar[1]=basamaklar[1] & 0xFB;else basamaklar[1]=basamaklar[1] | 0x04;
            if(mhz_artma)basamaklar[4]=basamaklar[4] & 0xFB;
         }
   }

