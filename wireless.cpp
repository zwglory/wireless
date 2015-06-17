//========================================================================
//	wireless.cpp:  Simulation of Spinal Codes over AWGN, without itpp.
//
//	Author: baobao7766
//	Time:   2015/04/22
//========================================================================

#include <fstream>			//std::ifstream, ofstream, seekg(), tellg(), read()
#include <iostream>			//std::cout, cin
#include <iterator>			//std::back_inserter
#include <vector>

#include "SpineEncoder.h"	//SpineEncoder::encoder(), sym_len
#include "SpineDecoder.h"	//SpineDecoder::decoder()
#include "WirelessType.h"

#include "base\signals.h"
#include "base\help_functions.h"

#include "Channel\AWGN.h"
#include "Channel\BERC.h"

using namespace std;

int main(int argc, char **argv)
{
	/* scalars and vectors */
	int k = 3,
		c = 8,
		precision = 8,
		L = 2,
		B = 4,
		d = 2;

	/* 1. Read message, getting vector and length. ***************************/

	cout << "Reading data >>>" << endl;
	int frameNum = 90;
	vector<int> message = Read_message("TestFile.txt", "binary.txt");
	//vector<int> message = Read_frame("coastguard_qcif.yuv", frameNum);
	int mlen = int(message.size());

	/* 2. Channel coding with spinal codes ***********************************/

	cout << '\n' << "SpineEncode start >>>" << endl;
	int spineLen = (mlen+k-1)/k;
	vec sym_trans;

	ofstream sym_file("Data\\symbol.txt", ios::binary);

	SpineEncoder sc(k, c, precision, L, mlen);
	sym_trans = sc.encoder(message);

	vec::iterator isym = sym_trans.begin();
	while(isym != sym_trans.end()){
		//cout << *isym << '\t';
		sym_file << *isym << '\t';
		++isym;
	}//end while
	cout << endl;

	/* Release files */
	sym_file.close();
	cout << "Symbol writing complete." << endl;
	cout << "Symbol length: " << L*spineLen << " byte" << endl
		 << "Spinal coding complete." << endl;

	/* 3. Symbols in the air *************************************************/

	cout << '\n' << "Channel simulation start >>>" << endl;
	/* Channel scalars and vectors */
	double Ec, Eb;
	vec EbN0dB, EbN0, N0, noise_variance, bit_error_rate; 

	AWGN awgn;		//The AWGN channel class
	BERC berc;		//Used to count the bit errors

	/* Init */
	Ec = sc.power;				//The transmitted energy per symbol.
	Eb = 1;					//The transmitted energy per bit.
	EbN0dB = linspace(0,3,10);	//Simulate for 10 Eb/N0 values from 0 to 9 dB.
	EbN0 = inv_dB(EbN0dB);		//Calculate Eb/N0 in a linear scale instead of dB. 
	N0 = get_N0(EbN0,Eb);		//N0 is the variance of the (complex valued) noise.
	
	for(int i=0; i!=EbN0dB.size(); ++i){
		//Show how the simulation progresses:
		cout << "Now simulating Eb/N0 value number " << i+1 << " of " << EbN0dB.size() << endl;

		//Set the noise variance of the AWGN channel:
		cout << "Noise: " << N0[i] << endl;
		awgn.setup(N0[i]);

		//Get the channel symbol we have recived.
		vec sym_recv = awgn.channel(sym_trans);

		/* 4. Decoding, sequence of 1 and 0, after decoded.**********************************/

		SpineDecoder desc(sym_recv, k, B, d, spineLen, L);
		bvec de_message = desc.decoder();

		// Check if de_message and message in the same length.
		if(de_message.size() != mlen){
			for(size_t count = de_message.size();
				count != mlen;
				--count)
				de_message.erase(de_message.end()-1);
		}//end if


		/* 5. Calculate the bit error rate. ***************************************************/

		berc.clear();									//Clear the bit error rate counter
		berc.count(message,de_message);				//Count the bit errors
		bit_error_rate.push_back(berc.get_errorrate());	//Save the estimated BER in the result vector
		//berc.report();

		if(!sym_recv.empty())
			sym_recv.clear();
		if(!de_message.empty())
			de_message.clear();
	}//end for

	//Print the results:
	cout << "Eb/N0	=	" << endl;
	print_vector(EbN0dB);
	save_vector(EbN0dB, "EbN0dB", "results");
	cout << "BER	=	" << endl;
	print_vector(bit_error_rate);
	save_vector(bit_error_rate, "BER", "results");

	system("pause");
	return 0;
}

