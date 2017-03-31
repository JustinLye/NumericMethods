#if !defined(__matrixBuffer_h__)
#define __matrixBuffer_h__
#include<iostream>
#include<fstream>
#include<Dense>
#include"nmutility.h"
namespace nm {
	typedef Eigen::Matrix<long double, Eigen::Dynamic, Eigen::Dynamic> MatrixXld;

	//This code is only used to read in the matrices.
	enum CHAR_TYPE {DIGIT, PERIOD, SPACE, NEWLINE, NULL_CHAR, PLUS_SIGN, MINUS_SIGN, OTHER};
	CHAR_TYPE GetCharType(unsigned int _asciiVal) {
			switch(_asciiVal) {
			case 0:
				return NULL_CHAR;
 				break;
			case 10:
			case 12:
				return NEWLINE;
				break;
			case 32:
				return SPACE;
				break;
			case 43:
				return PLUS_SIGN;
				break;
			case 45:
				return MINUS_SIGN;
				break;
			case 46:
				return PERIOD;
				break;
			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:
			case 101:
				return DIGIT;
				break;
			default:
				return OTHER;
				break;
			}
		}
	CHAR_TYPE GetCharType(const char _character) { return GetCharType((unsigned int)_character); }
	class inputBuffer {
	protected:
		std::string _buffer;
		unsigned int _size;
		unsigned int _capacity;
		long double* _inputValues;
		void _resize() {
			long double* _temp = _inputValues;
			_capacity *= 2;
			_inputValues = new long double[_capacity];
			for(unsigned int i = 0; i < _size; i++)
				_inputValues[i] = _temp[i];
			delete[] _temp;
		}


	public:
		inputBuffer() : _capacity(16), _size(0), _inputValues(new long double[_capacity]) {}
		void bufferChar(const char _inputChar) {
			_buffer.append(1, _inputChar);
		}
		~inputBuffer() { delete[] _inputValues; }

		void toValue() {
			long double _tempVal = std::stold(_buffer.c_str(),nullptr);
			_buffer.clear();
			if(_size == _capacity)
				_resize();
			_inputValues[_size++] = _tempVal;
		}
		void clear() { 
			_buffer.clear();
			_capacity = 32;
			_size = 0;
			long double* _temp = _inputValues;
			_inputValues = new long double [_capacity];
			delete[] _temp;
		}
		unsigned int size() const { return _size; }
		bool isEmpty() const { return (_size == 0); }
		bool isBufferEmpty() const { return _buffer.empty(); }
		long double& operator[](unsigned int _index) {
			return _inputValues[_index];
		}
	};
	Eigen::MatrixXd* readInput(const char*, nm::inputBuffer&);
	Eigen::MatrixXd* readInput(const char* _fileName,nm::inputBuffer& _inputBuffer) {
		std::ifstream _inputFile(_fileName);
		if(!_inputFile)
			throw std::exception("failure:\t inputfile could not be opened");
		if(!_inputBuffer.isEmpty())
			_inputBuffer.clear();
		std::string _input;
		nm::CHAR_TYPE _lastCharType;
		bool unknownColumns = true;
		unsigned int _rowCount = 0, _columnCount = 0, _totalColumns = 0;
		char _tempChar = NULL;
		char _lastChar = NULL;
		while(_inputFile) {
			_inputFile.get(_tempChar);
			if(_inputFile.eof() && _lastCharType != NEWLINE && _lastCharType != SPACE) {
				_rowCount++;
				_columnCount++;
				if(!unknownColumns && _totalColumns != _columnCount) {
					std::string _errMsg = "failure:\tinconsistent number of columns found in input file:\t" + std::string(_fileName);
					throw std::exception(_errMsg.c_str());
				} else if(unknownColumns) {
					_totalColumns = _columnCount;
					unknownColumns = false;
				}			
				_inputBuffer.toValue();
				break;
			}
			_lastCharType = GetCharType(_tempChar);
			switch (_lastCharType)
			{
			case DIGIT:
			case PERIOD:
			case NULL_CHAR:
			case PLUS_SIGN:
			case MINUS_SIGN:
				_inputBuffer.bufferChar(_tempChar);
				break;
			case SPACE:
				if(!_inputBuffer.isBufferEmpty() && GetCharType(_lastChar) != SPACE) {
					_columnCount++;
					_inputBuffer.toValue();
				}
				break;
			case NEWLINE:
				if(!_inputBuffer.isBufferEmpty()) {
					_rowCount++;
					_columnCount++;
					if(!unknownColumns && _totalColumns != _columnCount) {
						std::string _errMsg = "failure:\tinconsistent number of columns found in input file:\t" + std::string(_fileName);
						throw std::exception(_errMsg.c_str());
					} else if(unknownColumns) {
						_totalColumns = _columnCount;
						unknownColumns = false;
					}
					_columnCount = 0;
					_inputBuffer.toValue();
				} else if(!_inputBuffer.isEmpty()) {
					if(unknownColumns) {
						_totalColumns = _columnCount;
						unknownColumns = false;
					}
					if(_inputFile.peek() != EOF)
						_rowCount++;
					_columnCount = 0;
				}
				
				break;
			case OTHER:
			default:
				std::string _errMsg = "failure:\tinvalid character found in input file:\t" + std::string(_fileName);
				throw std::exception(_errMsg.c_str());
				break;
			}
			_lastChar = _tempChar;

		}
		_inputFile.close();
		Eigen::MatrixXd* _matrix = new Eigen::MatrixXd(_rowCount, _totalColumns);
		unsigned int rc = 0, cc = 0;
		for(unsigned int i = 0; i < _inputBuffer.size(); i++) {
			if(cc == _totalColumns) {
				cc = 0;
				rc++;
			}
			_matrix->operator()(rc, cc) = _inputBuffer[i];
			cc++;
		}
		_inputBuffer.clear();
		return _matrix;
	}

	Eigen::MatrixXd* gramm(const Eigen::MatrixXd* _A) {
		Eigen::MatrixXd* G = new Eigen::MatrixXd;
		*G = *_A;
		*G = G->transpose() * *G;
		return G;
	}

	Eigen::MatrixXd* gramm(const char* _fileName, nm::inputBuffer& _inputBuffer) {
		return gramm(nm::readInput(_fileName, _inputBuffer));
	}

}

#endif