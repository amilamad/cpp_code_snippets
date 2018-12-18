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
const int TypeIfo::Registered<T>::index0 = TypeIfo::addIndex();

__declspec(selectany) int TypeIfo::nextIndex = 0;
 
/** 
Usage :
int idx0 = TypeIfo::getIndex<MapPlane>();
int idx1 = TypeIfo::getIndex<std::string>();
int idx2 = TypeIfo::getIndex<MapPlane>();
*/
