#include "JsonHelper.h"
int JsonHelper::countSplittedSize(MyString** pathSplited)
{
	int count = 0;
	while (*pathSplited != nullptr)
	{
		count++;
		pathSplited++;
	}
	return count;
}

void JsonHelper::free(MyString** pathSplited, int count)
{
	for (int i = 0; i < count; i++)
	{
		delete pathSplited[i];
	}
	delete[] pathSplited;
}

int JsonHelper::toInt(const MyString& num)
{
	int result = 0;
	int size = num.getSize();
	if (num[0] == '-')
	{
		for (int i = 1; i < size; i++)
		{
			result = result * 10 + (num[i] - '0');
		}
		return result * (-1);
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			result = result * 10 + (num[i] - '0');
		}
		return result;
	}
}