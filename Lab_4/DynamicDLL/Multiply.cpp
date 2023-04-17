extern "C" __declspec(dllexport) int Mull(int a, int b)
{
    int c;
    __asm
    {
        mov eax, a
        mov ebx, b
        mul ebx
        mov c, eax
    }
    return c;//сборка на х86
    //return a * b;
}