#include <stdio.h>
#include <stdint.h>
#include "tableaux_de_permutation.h"
#include <time.h>

// Affiche un entier en binaire avec des espaces reguliers. Cette fonction permet de déboguer
// E: x: entier a afficher , nBits: nombre de bits a afficher , space: nombre de chiffres entre chaque espace
// S: vide
void int2bin(uint64_t x, int nBits, int space) {
	for( int i=nBits-1; i>=0; i--) {
		uint64_t temp = (unsigned long) 0b1<<i;
		printf("%i", (x&temp) != 0);

		if((i)%space == 0) printf(" ");
	}

	printf("\n");
}
void decimal_to_binary(unsigned long long int decimal)
{
    int tab[64];
    int i = 0;
    printf(" valeur decimal = %llu\n", decimal);
        for(i = 0; decimal>0;i++)
    {
        tab[i] = decimal%2;
        decimal = decimal/2;
    }
    printf(" valeur binaire = ");
    for(i-=1; i >= 0; i--)
    {
       printf("%u",tab[i]);
    }
    printf("\n");
}

// Effectue une permutation à partir d'un tableau de permutation défini dans "tableaux_de_permutation.h"
// E: in : bits en entree , tailleIn : nombre de bits en entree 
//  , tableauDePermuation : tableau d'entier , tailleOut : nombre de bits en sortie
// S: bits en sortie
uint64_t permutation(uint64_t in, int tailleIn ,const int* tblDePermutation, int tailleOut) {
	uint64_t res = 0;

	// tblDePermutation[sortie] = entree
	const uint64_t bitPoidsFortIn = (unsigned long) 0b1 << (tailleIn-1);
	const uint64_t bitPoidsFortOut = (unsigned long) 0b1 << (tailleOut-1);

	for(int i = 0; i<tailleOut; i++ ) {
		uint64_t temp = (unsigned long) bitPoidsFortIn >> (tblDePermutation[i]-1);
		if( (in&temp) != 0)
			res |= bitPoidsFortOut >> i;
	}

	return res;
}


// Effectue une la substitution a partir des Sbox definis dans "tableaux_de_permutation.h"
// E: 48 bits
// S: 32 bits
uint32_t substitution(uint64_t data_48b) {
    uint32_t res = 0;

    for(unsigned int i = 0; i < 8; i++)
    {
        uint8_t entree_sbox_6bit = 0b111111 & ( data_48b >> 6*(7-i) );
        char premier_bit = (entree_sbox_6bit >> 5) & 0b1;
        char dernier_bit = entree_sbox_6bit        & 0b1;
        char bits_externes = (premier_bit<<1) + dernier_bit;
        char bits_milieu = (entree_sbox_6bit & 0b011110 ) >> 1;
        //printf("sbox %u \n", i);)
        //printf("    ligne : %u \n", bits_externes);
        //printf("    colonne : %u \n", bits_milieu);
        res |= sbox[i][bits_externes][bits_milieu] << 4*(7-i);
        //printf("res = %u \n",res[i] );
    }

    return res;
}


// Realise une expansion
// E: 32 bits
// S: 48 bits
uint64_t expansion(unsigned long int data_32b) {
    uint64_t res = 0;

    for(int i = 0; i<8; i++) {
        uint8_t in4bit = (data_32b >> (28-i*4)) & 0b1111;
        uint8_t out6bit = 0;

        uint8_t bitPoidsFort;
        uint8_t bitPoidsFaible;
        if(i==0)
            bitPoidsFort  = (data_32b >> 0      )   & 0b1;
        else
            bitPoidsFort  = (data_32b >> (4*(8-i)) )& 0b1;
        if(i==7)
            bitPoidsFaible= (data_32b >> 31     )   & 0b1;
        else
            bitPoidsFaible= (data_32b >> (27-i*4) ) & 0b1;

        out6bit = (bitPoidsFort << 5) | (in4bit << 1) | bitPoidsFaible; 

        res |= (unsigned long long) out6bit << (42-i*6);
    }

    return res;
}

uint32_t DES_function(uint32_t data_32b, uint64_t subkey_48b) {
	uint32_t temp = substitution( expansion(data_32b) ^ subkey_48b);
	//uint32_t res = straightPermutation( substitution( expansion(data_32b) ^ key_48b));
	uint32_t res = permutation(temp, 32, straight_Dbox, 32); 
    return res;
}

// 
//
//
uint64_t feistel_round(uint64_t data_64b, uint64_t subkey_48b) {
	static const uint32_t msq_half = 0xFFFFFFFF;

    uint32_t left_part = (data_64b>>32);
    uint32_t right_part = (data_64b)&msq_half;
    /*
    printf("left_part \n");
    decimal_to_binary(left_part);
    printf("right_part \n");
    decimal_to_binary(right_part);
	*/
    uint32_t new_left_part = right_part;
    uint32_t new_right_part = left_part ^ (DES_function(right_part,subkey_48b));

    uint64_t res = new_right_part + ((unsigned long long) new_left_part<<32);
    /*
    printf("new_left_part \n");
    decimal_to_binary(new_left_part);
    printf("new_right_part \n");
    decimal_to_binary(new_right_part);
    printf(" concatenation\n");
    decimal_to_binary(res);
	*/

    return res;
}

// Decale tous les bits vers la gauche, 1 ou 2 fois selon la valeur de round
// E: 28 bits, entier round
// S: 28 bits
uint32_t shift_left(uint32_t data_28b, unsigned int round)
{
    static const uint32_t msq_28b = 0x0FFFFFFF;
    data_28b &= msq_28b;

    uint32_t res = 0;

    if(round == 1 || round == 2 || round == 9 || round == 16)
    {
        uint8_t premier_bit = (data_28b>>27);

        res = (data_28b<<1) + premier_bit;
    }
    else
    {
        uint8_t deux_premier_bit = (data_28b>>26);
        res = (data_28b<<2) + deux_premier_bit;
    }
    res &= msq_28b;

    return res;
}


void keyGeneration(uint64_t key_64b, uint64_t* roundKeys) {
	uint64_t temp = permutation(key_64b, 64, parity_drop, 56);

	uint32_t b28_l,b28_r; // Troncqge sur 28 bits
	const uint32_t msq_28b = 0x0FFFFFFF;
	b28_l = (temp >> 28) & msq_28b;
	b28_r = temp & 0x0FFFFFFF;

	for(int i=1; i<=16; i++) {
		b28_l = shift_left(b28_l, i);
		b28_r = shift_left(b28_r, i);
		uint64_t inDbox = ( (unsigned long) b28_l << 28) + b28_r;

		roundKeys[i-1] = permutation(inDbox, 56, compression_Dbox, 48);
	}

}

uint64_t DES_cipher(uint64_t data_64b, uint64_t key, int cipher) { // 0: chiffrer / 1:dechiffrer
	uint64_t roundKeys[16] = {0};
	keyGeneration(key, roundKeys);

	data_64b = permutation(data_64b, 64, initial_perm, 64);

	//printf("init_perm:\t\t%16llX\n",data_64b);

	for(int i=0; i<16; i++) {
		if(cipher == 0)
			data_64b = feistel_round(data_64b, roundKeys[i]);
		else
			data_64b = feistel_round(data_64b, roundKeys[15-i]);
		//printf("round %i:\t\t%16llX\n",i ,data_64b);
	}

	uint32_t left_part = (data_64b>>32);
    uint32_t right_part = (data_64b)&0xFFFFFFFF; 

    data_64b = ( (unsigned long long) right_part<<32) + (left_part);

	data_64b = permutation(data_64b, 64, final_perm, 64);

	return data_64b;
}

//TEST
int main() {
	// Test Feistel round
	/*
	uint64_t in = 0x5A78E3944A1210F6; // 64bit
	uint64_t key= 0x000006EDA4ACF5B5; // 48bit
	uint64_t out = 1; //64bit

	printf("in:\t\t%16llX\n",in); 
	printf("key:\t\t%016llX\n",key); 
	
	out = feistel_diagram(in, key);

	printf("out:\t\t%016llX\n", out);
	*/

	// Test Key generation
	/*
	uint64_t inkey= 0xAABB09182736CCDD; 
	uint64_t outkey = 1; 

	uint64_t tbl[16] = {0};
	keyGeneration(inkey, tbl);
	for(int i=0; i<16; i++)
		printf("key%i:\t\t%16llX\n",i,tbl[i]);
	*/

	// Test du chiffrement
	/*
	uint64_t in = 0x123456ABCD132536; 
	uint64_t key= 0xAABB09182736CCDD; 
	uint64_t out = 1; 

	printf("--CIPHER--\n"); 
	printf("in:\t\t%16llX\n",in); 
	printf("key:\t\t%016llX\n",key); 
	
	out = DES_cipher(in, key, 0);

	printf("out:\t\t%016llX\n", out);
	
	printf("--DECIPHER--\n"); 

	out = DES_cipher(out, key, 1);

	printf("out:\t\t%016llX\n", out);
*/

	// Calcul de la vitesse de chiffrement
	uint64_t in = 0x123456ABCD132536; 
	uint64_t key= 0xAABB09182736CCDD; 
	
	clock_t t = clock();
	

	for(long int i = 0; i < 1e3; i++) {
		DES_cipher(in,key,0);
	}

	t = clock()-t;
	float tempsEnSeconde = ((float) t)/CLOCKS_PER_SEC;
	printf("%f\n", tempsEnSeconde);

	printf("%f MO/s\n", 8.f/(tempsEnSeconde));


	return 0;
}

// Site web pour tester les resultats http://des.online-domain-tools.com/
