//---------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <string>
#pragma hdrstop
using namespace std;
   unsigned char base[] = {

      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',

      'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',

      'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',

      'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',

      'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',

      'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',

       '8', '9', '+', '/'};
//---------------------------------------------------------------------------

#pragma argsused
bool isb64code(unsigned char ch)
{
    for(int i=0;i<64;i++)
    if(base[i] == ch)
     return true;
   return false;
}
unsigned char decode2(unsigned char c)
 {
   for(int i=0;i<64;i++)
    if(base[i] == c)
     return i;
   return 255;
}
unsigned char decode(unsigned char c)
 {
  if(c>=65 && c<=90) return c-65;else
  if(c>=97 && c<=122) return c-71;else
  if(c>=48 && c<=57) return c+4;else
  if(c==43) return 62;else
  if(c==47) return 63;
  return 255;
 }
void asemble_byte(unsigned char  bytes[3], unsigned char wej[4])
 {
 //dekodowanie zablicy danych wejsciowych
 for(int i=0;i<4;i++)
   wej[i] = decode2(wej[i]);
 //zerowanie tablicy wyjsciowej
  for(int i=0;i<3;i++)
   bytes[i] = 0;
 //ustawienie pierwszego bajtu danych
    bytes[0] = wej[0] << 2;
    bytes[0] = bytes[0] | ( wej[1] >> 4 );
 //ustawienie drugiego bajtu danych
    bytes[1] = wej[1] << 4;
    bytes[1] = bytes[1] | (wej[2] >> 2);
 //ustawienie 3 bajtu danych
    bytes[2] = wej[2] << 6;
    bytes[2] = bytes[2] | wej[3];
 }
int main(int argc, char* argv[])
{
std::fstream wej;
std::fstream wyj;

if(argc!=3)
 {
  std::cout << "Program wymaga podania 2 argumentow." << std::endl;
  std::cout << "b64decode plik_wej plik_wyj" << std::endl;
  return 1;
 }

wej.open( argv[1], std::ios::in);
wyj.open( argv[2], std::ios::out | std::ios::trunc);
if(wej.good() && wyj.good())
 {
  char buf [4];
  char c;
  unsigned char bin[ 3 ];
  int count;
   do{
        count = 0;
        while( (!wej.eof() ) && (count<4) )
        {
         wej.get(c);
         if(isb64code(c))
          buf[count++] = c;
        }
    asemble_byte( bin, buf);
    wyj.write(bin, count-1);
   }while (count>0);

  wej.close();
  wyj.close();
 }else
 {
  std::cout << "Wystapily problemy podczas otwierania plikow, spradz czy nazwy podane sa prawidlowo." << std::endl;
  wej.close();
  wyj.close();
  return 1;
 }

return 0;
}
//---------------------------------------------------------------------------
 