#include <stdio.h>
#include <string.h>

#define MAX_BUF  256

#define IV  0b10110001
#define KEY 0b11001011
#define CTR 0b00110101

void encode(unsigned char*, unsigned char*, int);
void decode(unsigned char*, unsigned char*, int);

unsigned char processCtr(unsigned char, unsigned char);
unsigned char encryptByte(unsigned char, unsigned char, unsigned char);
unsigned char decryptByte(unsigned char, unsigned char, unsigned char);

unsigned char getBit(unsigned char, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);


int main()
{
  char str[8];
  int  choice;

  printf("\nYou may:\n");
  printf("  (1) Encrypt a message \n"); //printing out the options
  printf("  (2) Decrypt a message \n");//printing out the options
  printf("\n  what is your selection: ");
  fgets(str, sizeof(str), stdin);
  sscanf(str, "%d", &choice);

  switch (choice) {

    case 1://if the user selects 1, the program will encrypt the message
      {
      char plaintext[MAX_BUF];//declaring the plaintext array to store the message
    	printf("Enter a plaintext string: ");
    	fgets(plaintext, sizeof(plaintext), stdin);//getting the message from the user
    	unsigned char ciphertext[MAX_BUF];//declaring the ciphertext array to store the encrypted message
    	encode(plaintext, ciphertext, strlen(plaintext));//calling the encode function to encrypt the message
    	for(int i = 0; i < strlen(plaintext); i++){
      		printf("%d ", ciphertext[i]);//printing out the encrypted message
    	}
    	printf("\n");
    	break;
      }

    case 2://if the user selects 2, the program will decrypt the decimal message
      {
      int num;//declaring the num variable to store the number of bytes in the message
    	int i = 0;//declaring the i variable to use as number of bytes in the message
    	unsigned char ciphertext[MAX_BUF];//declaring the ciphertext array to store the encrypted message
   	printf("Enter decimals to decrypt: ");
    	while(num != -1){//getting decimals from the user until user enters -1
      	scanf("%d", &num);//getting the number from the user
      	ciphertext[i] = num;//storing the number in the ciphertext array
      	i++;//incrementing the i variable value by 1
    	}
    	unsigned char plaintext[MAX_BUF];//declaring the plaintext array to store the decrypted message
    	decode(ciphertext, plaintext, i);//calling the decode function to decrypt the message
    	printf("Decrypted decimals: %s\n", plaintext);//printing out the decrypted message
   	break;
      	}
  }

  return(0);
}

/*
  Function:  getBit
  Purpose:   retrieve value of bit at specified position
       in:   character from which a bit will be returned
       in:   position of bit to be returned
   return:   value of bit n in character c (0 or 1)
*/
unsigned char getBit(unsigned char c, int n){
  char b = 0;
  b = (1 & (c >> n));
  return b;
}

/*
  Function:  setBit
  Purpose:   set specified bit to 1
       in:   character in which a bit will be set to 1
       in:   position of bit to be set to 1
   return:   new value of character c with bit n set to 1
*/
unsigned char setBit(unsigned char c, int n){
  (c |= (1 << n));
  return c;
}

/*  Function:  clearBit
  Purpose:   set specified bit to 0
       in:   character in which a bit will be set to 0
       in:   position of bit to be set to 0
   return:   new value of character c with bit n set to 0
*/
unsigned char clearBit(unsigned char c, int n){
  (c &= ~(1 << n));
  return c;
}


unsigned char processCtr(unsigned char ctr, unsigned char key){
  unsigned char tempCtr = ctr; //copying counter value into a temporary counter
  int startBit; //initializing startBit
  if (ctr % 2 == 0){
    startBit = 0; //if the counter is even, start at bit 0
  } else {
    startBit = 1;//if the counter is odd, start at bit 1
  }
  for (int i = startBit; i < 8; i += 2){ //looping through the bits of the counter
    unsigned char xor = getBit(ctr, i) ^ getBit(key, i);//Perform an xor operation between the current bits of the counter and of the key
    if (xor == 1){
      tempCtr = setBit(tempCtr, i);//if the result of the xor is 1, set the current bit of the counter to 1
    } else {
      tempCtr = clearBit(tempCtr, i);//else clear the current bit of the counter
    }
  }
  return tempCtr;//return the temporary counter value as the updated counter 
}

unsigned char encryptByte(unsigned char pt, unsigned char ctr, unsigned char prev){
  unsigned char temp = 0; //initializing a temp byte to zero
  for (int i = 0; i < 8; i++){ //looping through the bits of the plaintext
    if (getBit(ctr, i) == 1){ //if the current bit of the counter is 1
      unsigned char xor = getBit(pt, i) ^ getBit(prev, i);//perform an xor operation between the current bit of the plaintext and the current bit of the previous ciphertext
      if (xor == 1){
        temp = setBit(temp, i);//if the result of the xor is 1, set the current bit of the temp byte to 1
      } else {
        temp = clearBit(temp, i);//else clear the current bit of the temp byte
      }
    } else {
      unsigned char xor = getBit(pt, i) ^ getBit(prev, 7 - i);//else perform an xor operation between the current bit of the plaintext and the mirror bit of the previous ciphertext byte
      if (xor == 1){
        temp = setBit(temp, i);//if the result of the xor is 1, set the current bit of the temp byte to 1
      } else {
        temp = clearBit(temp, i);//else clear the current bit of the temp byte
      }
    }
  }
  return temp;//return the temp byte value as the ciphertext byte
}

void encode(unsigned char *pt, unsigned char *ct, int numBytes){
  unsigned char ctr = CTR; //copying the counter value into a temporary counter
  unsigned char prev = 0; //initializing the previous ciphertext byte to zero
  for (int i = 0; i < numBytes; i++){//looping through the plaintext bytes
    ctr = processCtr(ctr, KEY);//updating the counter value
    ct[i] = encryptByte(pt[i], ctr, prev);//encrypting the current plaintext byte
    prev = ct[i];//updating the previous ciphertext byte
    ctr++;//incrementing the updated counter value by 1
  }
}

unsigned char decryptByte(unsigned char ct, unsigned char ctr, unsigned char prev){
  unsigned char temp = 0; //initializing a temp byte to zero
  for (int i = 0; i < 8; i++){//looping through the bits of the ciphertext
    if (getBit(ctr, i) == 1){//if the current bit of the counter is 1
      unsigned char xor = getBit(ct, i) ^ getBit(prev, i);//perform an xor operation between the current bit of the ciphertext and the current bit of the previous ciphertext
      if (xor == 1){
        temp = setBit(temp, i);//if the result of the xor is 1, set the current bit of the temp byte to 1
      } 
      else {
        temp = clearBit(temp, i);//else clear the current bit of the temp byte
      }
    } 
    else {
      unsigned char xor = getBit(ct, i) ^ getBit(prev, 7 - i); //else perform an xor operation between the current bit of the chipertext and the mirror bit of the previous 
      if (xor == 1){
        temp = setBit(temp, i);//if the result of the xor is 1, set the current bit of the temp byte to 1
      } 
      else {
        temp = clearBit(temp, i);//else clear the current bit of the temp byte
      }
    }
  }
  return temp;//return the temp byte value as the plaintext byte
}

void decode(unsigned char *ct, unsigned char *pt, int numBytes){
  unsigned char ctr = CTR;//copying the counter value into a temporary counter
  unsigned char prev = 0;//initializing the previous ciphertext byte to zero
  for (int i = 0; i < numBytes; i++){//looping through the ciphertext bytes
    ctr = processCtr(ctr, KEY);//updating the counter value
    pt[i] = decryptByte(ct[i], ctr, prev);//decrypting the current ciphertext byte into its corresponding plaintext byte, and stores the decrypted byte into the plaintext array pt
    prev = ct[i];//updating the previous ciphertext byte
    ctr++;//incrementing the updated counter value by 1
  }
}


