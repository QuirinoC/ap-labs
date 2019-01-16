def solution(s):
    #Return 0 if len 0
    if len(s) == 0: return 0
    #Return 1 if len 1
    if len(s) == 1: return 1
    #Bit vector as memory
    mem = 1 << (ord(s[0]) - ord('A'))
    #Left pointer
    l = 0
    #Max size
    size = 1
    for r in range(1, len(s)):
        #Check if repeated char is found
        if mem & 1 << ( ord(s[r]) - ord('A') ):
            #Check if substring is bigger
            tmp = r - l
            size = tmp if tmp > size else size
            #Move the left index until the substring is unique
            for i in range(l, r):
                #Turn off the bits until repeated is found
                if s[i] != s[r]:
                    mem = mem & ~(1 << (ord(s[i]) - ord('A')))
                l += 1
                if s[i] == s[r]:
                    break
        #Turn on the bit on the kth position
        mem |= 1 << ( ord(s[r]) - ord('A') )
    #Case if the substring is at the end
    r += 1
    tmp = r - l
    size = tmp if tmp > size else size  

    return size   
    
u = input("String: ")
print("Result:",solution(u))