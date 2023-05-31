extern "C" __declspec(dllexport) int Mull(int a, int b)
{
    ////int c;
    //__asm
    //{
    //    mov eax, a
    //    mov ebx, b
    //    mul ebx
    //    
    //}
    ////return c;//������ �� �86 mov c, eax
    //return a * b;
    __asm 
    {
        ; ������� ��������� ���� �����
        ; ������� ��������� :
        ; a - ������ ���������(���������� � �������� EAX)
        ; b - ������ ���������(���������� � �������� EBX)
        ; �������� ��������� :
        ; ��������� ���������(������������ � �������� EAX)

        multiply:
            push ebp; ��������� ������� �������� ��������� �� �������� �����
            mov ebp, esp; ������������� ����� �������� ��������� �� �������� �����

            mov eax, [ebp + 8]; ��������� ������ ��������� �� ����� � ������� EAX
            mov ebx, [ebp + 12]; ��������� ������ ��������� �� ����� � ������� EBX

            imul ebx; �������� EAX �� EBX

            mov esp, ebp; ��������������� ��������� �� �������� �����
            pop ebp; ��������������� ���������� �������� ��������� �� �������� �����
    }
    return a * b;
}