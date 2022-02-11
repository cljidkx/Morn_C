// g++ -O2 -DNDEBUG test_text2.cpp -o test_text2.exe -lmorn

#include "morn_util.h"
#include <string>

void test(const char *find)
{
    printf("\n%s\n",find);
    
    int num;
    MArray *text= mTextCreate();
    mFileText(text,"D:/Morn/test/test/a_Q_zheng_zhuan.txt");

    num=0;
    mTimerBegin("strstr");
    for(int i=0;i<10000;i++)
    {
        char *p_text = text->text;
        while(1)
        {
            p_text=strstr(p_text,find);
            if(p_text==NULL) break;
            num++;
            p_text=p_text+1;
        }
    }
    mTimerEnd("strstr");
    printf("num=%d\n",num);

    num=0;
    std::string string(text->text,text->num);
    mTimerBegin("stl");
    for(int i=0;i<10000;i++)
    {
        int pos=0;
        while(1)
        {
            pos=string.find(find,pos+1);
            if (pos == std::string::npos) break;
            num++;
        }
    }
    mTimerEnd("stl");
    printf("num=%d\n",num);

    num=0;
    MArray *str = mTextCreate(find);
    mTimerBegin("Morn");
    for(int i=0;i<10000;i++)
    {
        int pos=0;
        while(1)
        {
            pos = mTextFind(text,str,pos+1);
            if(pos<0) break;
            num++;
        }
    }
    mTimerEnd("Morn");
    printf("num=%d\n",num);

    mTextRelease(text);
    mTextRelease(str);
}

int main()
{
    test("��Q");
    test("������������!");
    test("��Qվ��һ�̣������룬�������㱻���Ӵ��ˣ����ڵ������治��������������Ҳ��������ĵ�ʤ�����ˡ�");
    test("��QҪ��ԲȦ�ˣ��������ű�ȴֻ�Ƕ�����������������ֽ���ڵ��ϣ���Q����ȥ��ʹ����ƽ��������ԲȦ�������±���Ц������־Ҫ����Բ������ɶ�ıʲ����ܳ��أ����Ҳ��������ո�һ��һ���ļ���Ҫ�Ϸ죬ȴ������һ�ʣ����ɹ���ģ���ˡ�");
    return 0;
}



