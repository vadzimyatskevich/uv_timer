REM       COMMAND FILE BUILT BY IDEA 
REM       Mon May 08 10:39:46 2017
REM
cxstm8 -v  +debug +modsl0 "uv_timer_vector.c"
cxstm8 -v  +debug +modsl0 "uv_main.c"
clnk -l"C:\Program Files (x86)\COSMIC\FSE_Compilers\CXSTM8\LIB" -o "uv_timer.sm8" "uv_timer.lkf"
