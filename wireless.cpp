//========================================================================
//	wireless.cpp:  Simulation of Spinal Codes over AWGN, without itpp.
//
//	Author: baobao7766
//	Time:   2015/02/19
//========================================================================

#include <fstream>			//std::ifstream, ofstream, seekg(), tellg(), read()
#include <iostream>			//std::cout, cin
#include <iterator>			//std::back_inserter

#include "SpineEncoder.h"	//SpineEncoder::encoder(), sym_len
#include "SpineDecoder.h"	//SpineDecoder::decoder()

#include "base\WirelessType.h"
#include "base\signals.h"

#include "Channel\AWGN.h"
#include "Channel\BERC.h"

using namespace std;

int main(int argc, char **argv)
{
	/* scalars and vectors */
	int k = 3,
		c = 8,
		precision = 8,
		L = 3,
		B = 4,
		d = 2;

	/* Read message */
	cout << "Reading data >>>" << endl;
	ifstream infile("Data\\TestFile.txt", ios::binary);
	ofstream outfile("Data\\binary.txt", ios::binary);
	
	/* Get length of file */
	infile.seekg (0, infile.end);
	int length = infile.tellg();
	infile.seekg (0, infile.beg);
	
	std::cout << "Reading " << length << " characters... " << endl;
	
	char* rawdata = new char [length];
	
	infile.read (rawdata,length);
	cout << "Content in message is: " << '\n' << rawdata << endl;

	cout << "Rawdata in binary is: " << endl;
	int mlen = length * 8;
	int* message = new int [mlen];
	for (int j=0; j!=length; ++j){
		for (int i=0; i != 8; ++i){
			message[i+j*8] = (((0x80 & (rawdata[j] << i)) == 128)?1:0);
			cout << message[i+j*8] << " ";	
			outfile << message[i+j*8];      // 文件对应的二进制结果输出到binary中
		}
	}
	cout << '\n' << "Writing complete." << endl;
	
	//===================================== 
	//	Channel coding with spinal codes 
	//=====================================
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
	}
	cout << endl;

	/* Release files */
	infile.close();
	outfile.close();
	sym_file.close();
	cout << "Symbol writing complete." << endl;
	cout << "Symbol length: " << L*spineLen << " byte" << endl
		 << "Spinal coding complete." << endl;

	/*SpineDecoder desc(sym_trans, k, B, d, spineLen, L);
	vector<int> de_message = desc.decoder();*/


	//============================================
	//	Symbols in the air 
	//============================================
	/* Channel scalars and vectors */
	double Ec, Eb;
	vec EbN0dB, EbN0, N0, noise_variance, bit_error_rate; 

	AWGN awgn;		//The AWGN channel class
	BERC berc;		//Used to count the bit errors
	bvec b_message(message, message + mlen);	//Show message into vector.

	/* Init */
	Ec = sc.power;				//The transmitted energy per symbol.
	Eb = 1;					//The transmitted energy per bit.
	EbN0dB = linspace(-1,1,5);	//Simulate for 10 Eb/N0 values from 0 to 9 dB.
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

		////Print recived symbols.
		//cout << "Recived symbols: " << endl;
		//vec::iterator it_rec = sym_recv.begin();
		//while(it_rec != sym_recv.end()){
		//	cout << *it_rec << ' ';
		//	++it_rec;
		//}
		//cout << endl;

		//Decoding, sequence of 1 and 0, after decoded.
		SpineDecoder desc(sym_recv, k, B, d, spineLen, L);
		bvec de_message = desc.decoder();

		// Check if de_message and message in the same length.
		if(de_message.size() != mlen){
			for(size_t count = de_message.size();
				count != mlen;
				--count)
				de_message.erase(de_message.end()-1);
		}//end if

		//Calculate the bit error rate:
		berc.clear();									//Clear the bit error rate counter
		berc.count(b_message,de_message);				//Count the bit errors
		bit_error_rate.push_back(berc.get_errorrate());	//Save the estimated BER in the result vector
		//berc.report();

		/* 要在这个地方加上与 BER 的阈值做比较的部分 */

		if(!sym_recv.empty())
			sym_recv.clear();
		if(!de_message.empty())
			de_message.clear();
	}//end for

	//Print the results:
	cout << "Eb/N0	=	" << endl;
	print_vector(EbN0dB);
	save_vector(EbN0dB, "results");
	cout << "BER	=	" << endl;
	print_vector(bit_error_rate);
	save_vector(bit_error_rate, "results");

	system("pause");

	return 0;
}

