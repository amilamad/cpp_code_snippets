/** [Windows]
This code can be used to get unique id for a class type or any data type. This will be helpfull in a system where no RTTI system available. 
*/

class TypeInfo
{
private:
	static int nextIndex;

	template <typename T>
	struct Registered
	{
		static const int index0;
	};


	static int addIndex()
	{
		return nextIndex++;
	}

public:
	template <typename T>
	static int getIndex()
	{
		return Registered<T>::index0;
	}
};

template <typename T>
const int TypeInfo::Registered<T>::index0 = TypeInfo::addIndex();

__declspec(selectany) int TypeInfo::nextIndex = 0;

/** 
Usage :
*/

int main()
{
	int idx0 = TypeInfo::getIndex<int>();
	int idx1 = TypeInfo::getIndex<std::string>();
	int idx2 = TypeInfo::getIndex<float>();

	return 0;
}

