#ifndef HELP_FUNCTION_H
#define HELP_FUNCTION_H

#include <fstream>
#include <iostream>
#include <string>
#include "..\WirelessType.h"

#define  unchar		unsigned char
#define  BlockSize  16

using namespace std;

/*******************************************************************
	为视频序列（如.yuv） new/delete 一个二维的空间（row * column）
********************************************************************/

static inline void new_uchar_array2D(unsigned char ***array2D, int row, int column,  const string &event_text){
	int i;
	string err_text = "Error in " + event_text;

	if ((*array2D = new unsigned char*[row]) == NULL){
		printf("error_in_allocating_mem");
	}else{
		if ( ((*array2D)[0] = new unsigned char[row * column]) == NULL){
			printf("error_in_allocating_mem");
		}//end if
	}//end if-else
	for (i = 1; i < row; i++){
		(*array2D)[i] = (*array2D)[i-1] + column;
	}//end for
}//new_uchar_array2D()


static inline void delete_uchar_array2D(unsigned char **array2D){
	if (array2D != NULL){
		if (*array2D != NULL){
			delete *array2D;
			*array2D = NULL;
		}//end if
		delete array2D;
	}//end if
}//end delete_uchar_array2D()


/*******************************************************************
	为视频序列（如.yuv） new/delete 一个二维的空间（row * column）
********************************************************************/

static inline void new_int_array2D(int ***array2D, int row, int column,  const string &event_text){
	int i;
	string err_text = "Error in " + event_text;
	if ((*array2D = new int*[row]) == NULL){
		printf("error in allocating memory");
	}else{
		if ( ((*array2D)[0] = new int[row * column]) == NULL){
			printf("error in allocating memory");
		}//end if
	}//end if-else
	for (i = 1; i < row; i++){
		(*array2D)[i] = (*array2D)[i-1] + column;
	}//end for
}//end new_int_array2D()


static inline void delete_int_array2D(int **array2D){
	if (array2D != NULL){
		if (*array2D != NULL){
			delete *array2D;
			*array2D = NULL;
		}//end if
		delete array2D;
	}//end if
}//end delete_int_array2D


/*******************************************************************
	为视频序列（如.yuv） new/delete 一个三维的空间（dim0 * dim1 * dim2）
********************************************************************/

static inline void new_int_array3D(int ****array3D, int dim0, int dim1, int dim2,  const string &event_text){
	int i;
	string err_text = "Error in" + event_text;

	if ((*array3D = new int **[dim0]) == NULL){
		printf("error in allocating memory");
	}else{
		new_int_array2D(*array3D, dim0 * dim1, dim2, event_text);
	}//end if-else

	for (i = 1; i < dim0; i++){
		(*array3D)[i] = (*array3D)[i-1] + dim1;
	}//end for
}//end new_int_array3D()

static inline void delete_int_array3D(int ***array3D){
	if (array3D != NULL){
		if (*array3D != NULL){
			delete_int_array2D(*array3D);
			*array3D = NULL;
		}//end if
		delete array3D;
	}//end if
}//end delete_int_array3D()


/*********************************************************************
	Print vector. 
**********************************************************************/

inline void print_vector(const bvec &in){
	bvec::const_iterator it = in.begin();
	while(it != in.end()){
		std::cout << *it << ' ';
		++it;
	}
	std::cout << std::endl;
}//end print_vector()

inline void print_vector(const vec &in){
	vec::const_iterator it = in.begin();
	while(it != in.end()){
		std::cout << *it << ' ';
		++it;
	}
	std::cout << std::endl;
}//end print_vector()


/*********************************************************************
	Print array. 
	问题：按字节打印会有问题。
**********************************************************************/

static inline void print_array(uint8_t* aryData, size_t len){
	size_t i = 0;
	for(i=0; i!=len; ++i){
		cout << aryData[i];
	}//end for
	cout << endl;
}//end print_array()

static inline void print_array(uint32_t* aryData, size_t len){
	size_t i = 0;
	for(i=0; i!=len; ++i){
		cout << hex << *(aryData+i) << '\t';
	}//end for
	cout << endl;
}//end print_array()


/*********************************************************************
	Save vector to file. 
	输出的文件存储在 Data 文件夹中，
	当 vector 名字为 "EbN0dB" 时，清空文件的内容
**********************************************************************/

inline void save_vector(const bvec &in, std::string vec_name, std::string filename){
	std::string path("Data\\");
	std::string extension(".txt");
	if(vec_name == "EbN0dB")
		std::ofstream fout(path + filename + extension, std::ios::trunc);
	std::ofstream fout(path + filename + extension, std::ios::app);
	fout << vec_name << ": " << std::endl;
	bvec::const_iterator it = in.begin();
	while(it != in.end()-1){
		fout << *it << ',';
		++it;
	}
	fout << *it << std::endl;
	if(fout){
		fout.clear();
		fout.close();
	}
	std::cout << "Writting file complete." << std::endl;
}//end save_vector()

inline void save_vector(const vec &in, std::string vec_name, std::string filename){
	std::string path("Results\\");
	std::string extension(".txt");
	if(vec_name == "EbN0dB")
		std::ofstream fout(path + filename + extension, std::ios::trunc);
	std::ofstream fout(path + filename + extension, std::ios::app);
	fout << vec_name << ": " << std::endl;
	vec::const_iterator it = in.begin();
	while(it != in.end()-1){
		fout << *it << ',';
		++it;
	}
	fout << *it << std::endl;
	if(fout){
		fout.clear();
		fout.close();
	}
	std::cout << "Writting file complete." << std::endl;
}//end save_vector()


/***************************************************************
	Read messages
*****************************************************************/

inline vector<int> Read_message(string filename, string binary_filename){
	
	ifstream infile("Data\\" + filename, ios::binary);
	ofstream outfile("Data\\" + binary_filename, ios::binary);
	
	/* Get length of file */
	infile.seekg (0, infile.end);
	int length = int(infile.tellg());
	infile.seekg (0, infile.beg);

	std::cout << "Reading " << length << " characters... " << endl;

	/* Read file into char */
	char* rawdata = new char [length];
	infile.read (rawdata,length);

	/*cout << "Content in message is: " << '\n' << rawdata << endl;
	cout << "Rawdata in binary is: " << endl;*/

	int mlen = length * 8;
	vector<int> message(mlen);

	for (int j=0; j!=length; ++j){
		for (int i=0; i != 8; ++i){
			message[i+j*8] = (((0x80 & (rawdata[j] << i)) == 128)?1:0);
			//cout << message[i+j*8] << " ";	
			outfile << message[i+j*8];      // 文件对应的二进制结果输出到binary中
		}
	}

	/* Release files */
	infile.close();
	outfile.close();
	delete [] rawdata;

	cout << "Writing complete." << endl;

	return message;
}//end Read_message()


/*******************************************************************************
	Read frame from input video.
********************************************************************************/

inline vector<int> Read_frame(string video_name, int frameNum){
	size_t file_type_index = video_name.find('.');
	if(video_name.substr(file_type_index) != ".yuv"){
		cerr << "Wrong video type! Please open a video." << endl;
	}//end if

	unsigned char **img_ref;	//2D reference
	new_uchar_array2D(&(img_ref), 144, 176, "alloc unchar for image buffer.");
	ifstream inVideoFile;

	inVideoFile.open(video_name, ios::in | ios::binary);
	inVideoFile.seekg(frameNum * 176 * 144 * 3 / 2, ifstream::beg);//第一个参数：偏移量，第二个：基地址

	for (int j = 0; j < 144; ++j){		
		inVideoFile.read((char *)img_ref[j], 176);//读取一帧Y图像
	}//end for

	vector<int> message;
	for(int j = 0; j != 144; ++j){
		for(int i = 0; i != 176; ++i){
			message.push_back(img_ref[j][i]);
		}//end for i
	}//end for j
	std::cout << "Reading " << 176*144 << " characters... " << endl;
	return message;

}//end Read_frame


/********************************************************************************
	vector<int>(bvec) to uint32_t
	----------------------------
	e.g.
		int a[] = 
		{
			1,0,0,1,1,1,1,0,
			0,0,0,1,0,1,1,0,
			1,0,1,0,0,0,1,1,
			1,0,1,0,0,1,0,1,

			1,1,0,0,0,0,1,0,
			1,1,1,1,0,0,0,0,
			1,0,1,1,1,1,0,0,
			0,0,1,0,1
		};
		pair<uint32_t*, int> va;
		bvec v(a,a+sizeof(a)/sizeof(int));
		va = bvec_uint32(v);

		print_vector(v);
		print_array(va.first, va.second);
	
**********************************************************************************/
static inline pair<uint32_t*, size_t> bvec_uint32(bvec vecData){
	int outLen = (vecData.size()+31)/32;
	uint32_t* output = new uint32_t [outLen];
	pair<uint32_t*, size_t> outpair(output, outLen);
	
	int i=0, j=0;
	while(i != outLen){
		*(output+i) = 0;
		++i;
	}//end while

	bvec::iterator it = vecData.begin();
	for(i=0; i != outLen; ++i){
		j = 0;
		while(it != vecData.end() && j!=32){
			*(output+i) = *it + (*(output+i) << 1);
			++it;
			++j;
		}//end while
	}//end for
	return outpair;
}

/********************************************************************************
	vector<int>(bvec) to uint8_t
	和 print_array 的 uint8_t 版本有待于进一步的调试。
**********************************************************************************/
static inline pair<uint8_t*, int> bvec_uint8(bvec vecData){
	int outLen = (vecData.size()+7)/8;
	uint8_t* output = new uint8_t [outLen];
	
	int i=0, j=0;
	while(i != outLen){
		*(output+i) = 0;
		++i;
	}//end while

	bvec::iterator it = vecData.begin();
	for(i=0; i != outLen; ++i){
		j = 0;
		while(it != vecData.end() && j!=8){
			*(output+i) = *it + (*(output+i) << 1);
			++it;
			++j;
		}//end while
	}//end for

	pair<uint8_t*, int> outpair(output, outLen);

	return outpair;
}

/***************************************************************
	Write bit vector.
*****************************************************************/
static inline void Write_bvec(bvec de_message){
	ofstream outfile("Results\\output.txt", ios::binary);
	pair<uint8_t*, int> dm = bvec_uint8(de_message);
	for(int ii=0; ii!=dm.second; ++ii){
		outfile << *(dm.first+ii);
	}
}//end Write_bvec()


#endif
