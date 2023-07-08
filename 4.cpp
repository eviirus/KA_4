#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	unsigned char reg[16] = { 0 }; char memory[256] = { 0 }; unsigned char flags = {0};
	
	//---read bin---//
	ifstream file;
	file.open("decryptor.bin", ios::in | ios::binary);
	file.unsetf(ios::skipws);

	int n = 0;
	while (!file.eof()) {
		file.get(memory[n]);
		n++;
	}
	
	//---main---//
	ifstream in("q1_encr.txt");
	ofstream out("q1.txt", ios::binary);
	int i = 0;
	while (1) {
		unsigned char num_x = memory[i + 1] & 0x0F;
		unsigned char num_y = (memory[i + 1] & 0xF0) >> 4;
		if ((unsigned char)memory[i] == 0x01) {
			reg[num_x] = reg[num_x]+1;
			i += 2;
		}
		else if((unsigned char)memory[i] == 0x02){
			reg[num_x] = reg[num_x] - 1;
			i += 2;
		}
		else if((unsigned char)memory[i] == 0x03) {
			reg[num_x] = reg[num_y];
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x04) {
			reg[0] = memory[i + 1]; 
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x05) {
			reg[num_x] = reg[num_x] << 1; 
			i += 2; 
		}
		else if ((unsigned char)memory[i] == 0x06) {
			reg[num_x] = reg[num_x]>>1;
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x07) {
			i += static_cast<int>(memory[i + 1]);
			while (i >= n) {
				i -= n; 
			}	
		}
		else if ((unsigned char)memory[i] == 0x08) {
			if ((flags & 0x1) == 0x1) {
				i += static_cast<int>(memory[i + 1]);
				while (i >= n) {
					i -= n;
				}
			}
			else i += 2;
		}
		else if ((unsigned char)memory[i] == 0x09) {
			if ((flags & 0x1) != 0x1) {
				i += static_cast<int>(memory[i + 1]);
				while (i >= n) {
					i -= n;
				}
			}
			else i += 2;
		}
		else if ((unsigned char)memory[i] == 0x0A) {
			if ((flags & 0x4) == 0x4) {
				i += static_cast<int>(memory[i + 1]);
				while (i >= n) {
					i -= n;
				}
			}
			else i += 2;
		}
		else if ((unsigned char)memory[i] == 0x0B) {
			return 0;
		}
		else if ((unsigned char)memory[i] == 0x0C) {
			reg[num_x] = reg[num_x] + reg[num_y];
			if (reg[num_x] == 0x0) {
				flags = flags | 0x1;
			}
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x0D) {
			reg[num_x] = reg[num_x] - reg[num_y]; 
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x0E) {
			reg[num_x] = reg[num_x] ^ reg[num_y];
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x0F) {
			reg[num_x] = reg[num_x] | reg[num_y];
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x10) {
			if (!in.eof()) {
				unsigned char input;
				in >> input; 
				int dec = static_cast<int>(input); 
				reg[num_x] = dec; 
			}
			else {
				flags = 0x4;
			}
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x11) {
			out << static_cast<char>(reg[num_x]);
			i += 2;
		}
		else if ((unsigned char)memory[i] == 0x12) {
			if ((flags & 0x2) == 0x2) {
				i += static_cast<int>(memory[i + 1]);
				while (i >= n) {
					i -= n;
				}
				flags = flags & 0xFD;
			}
			else i += 2;
		}
	}

	file.close();
	in.close();
}