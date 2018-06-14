
#include <xc.h>
#include "fmcw.h"

/* Do not change!! */
#define FMTX_   1
#define CWTX_   2
#define FMRX_   3

#include "time.h"

/*
 * �yFMCW�ݒ�̏������z
 *  FM��M�CFM���M�CCW���M���ꂼ���ݒ�
 *  1. CLK�[�q�i�N���b�N�j�CDAT�[�q�i�f�[�^�j�CSTB�[�q�i�X�g���[�u�j���o�͂Ƃ��Ďg�p
 *  2. �S�Ẵ|�[�g��Low�ɂ���
 */
void Init_FMCW(void){
    /* �|�[�g��Low�ɂ���i�������j */
    FMRX_CLK = 0;
    FMRX_DAT = 0;
    FMRX_STB = 0;
    FMTX_CLK = 0;
    FMTX_DAT = 0;
    FMTX_STB = 0;
    FMTX_PTT = 0;
    CWRX_CLK = 0;
    CWTX_DAT = 0;
    CWTX_STB = 0;
    CWTX_KEY = 0;
}


/*
 * �y�����@��'Low'�𑗂�z
 *  1. �ǂ̖����@�ɑ��邩�I���iFMTX or FMRX or CWTX�j
 *  2. DAT�[�q��Low�ɂ���
 *  3. CLK�[�q��0��1��0�ƕω������� 
 */
void L_OUT(int fmcwtxrx){
    if(fmcwtxrx == FMTX_)
    {
        FMTX_DAT = 0;
        _NOP();
        FMTX_CLK = 1;
        _NOP();
        FMTX_CLK = 0;
    }
    if(fmcwtxrx == CWTX_)
    {
        CWTX_DAT = 0;
        _NOP();
        CWRX_CLK = 1;
        _NOP();
        CWRX_CLK = 0;
    }
    if(fmcwtxrx == FMRX_)
    {
        FMRX_DAT = 0;
        _NOP();
        FMRX_CLK = 1;
        _NOP();
        FMRX_CLK = 0;
    }
}


/*
 * �y�����@��'High'�𑗂�z
 *  1. �ǂ̖����@�ɑ��邩�I���iFMTX or FMRX or CWTX�j
 *  2. DAT�[�q��High�ɂ���
 *  3. CLK�[�q��0��1��0�ƕω������� 
 */
void H_OUT(int fmcwtxrx){
    if(fmcwtxrx == FMTX_)
    {
        FMTX_DAT = 1;
        _NOP();
        FMTX_CLK = 1;
        _NOP();
        FMTX_CLK = 0;
    }
    if(fmcwtxrx == CWTX_)
    {
        CWTX_DAT = 1;
        _NOP();
        CWRX_CLK = 1;
        _NOP();
        CWRX_CLK = 0;
    }
    if(fmcwtxrx == FMRX_)
    {
        FMRX_DAT = 1;
        _NOP();
        FMRX_CLK = 1;
        _NOP();
        FMRX_CLK = 0;
    }
}


/*
 * �y�����@��STB�M���𑗂�z
 *  1. �ǂ̖����@�ɑ��邩�I���iFMTX or FMRX or CWTX�j
 *  2. STB�[�q��0��1��0�ƕω�������
 */
void STBOUT(int fmcwtxrx){
    if(fmcwtxrx == FMTX_)
    {
        FMTX_STB = 1;
        _NOP();
        FMTX_STB = 0;
    }
    if(fmcwtxrx == CWTX_)
    {
        CWTX_STB = 1;
        _NOP();
        CWTX_STB = 0;
    }
    if(fmcwtxrx == FMRX_)
    {
        FMRX_STB = 1;
        _NOP();
        FMRX_STB = 0;
    }
}


/*
 * �y�����@�̃v���O���}�u���J�E���^��ݒ肷��z
 *  1. ��������ǂݍ��񂾃v���O���}�u���J�E���^��2�i���ɕϊ��i�z��Ƃ��Ċi�[�j
 *  2. �i�[����2�i���ɍ��킹��High��Low�𖳐��@�ɑ���i�ݒ�̊́j
 *  3. �O���[�v�R�[�h�𑗂�'10'
 *  4. STB�M���𑗂�
 */
void OUTFQ(int fmcwtxrx, int *Nprg){
    int count = 0;
    int Nprg_b[17];
    
    for(int i=0; i<17; i++){
        Nprg_b[i] = 0;
    }
    
    //Nprg transforms decimal to binary
    for(int i = 0; i < 17; i++){
        for(int j = 0; j<5; j++){
            if(Nprg[j] % 2 == 0) {
                if(j == 4){
                    Nprg[j] = Nprg[j] / 2;
                    Nprg_b[count] = 0;
                    count++;
                }
                else{
                    Nprg[j] = Nprg[j] / 2;
                }
            }
            else if(Nprg[j] % 2 == 1) {
                if(j == 4){
                    Nprg[j] = Nprg[j] / 2;
                    Nprg_b[count] = 1;
                    count++;
                }
                else{
                    Nprg[j] = Nprg[j] / 2;
                    Nprg[j+1] = Nprg[j+1] + 10;
                }
            }
        }
    }
    
    //Send Nprg data(binay) to communication module
    for (int i=0; i<17; i++)
    {
        if(Nprg_b[i] == 0)
        {
            L_OUT(fmcwtxrx);
        }
        if(Nprg_b[i] == 1)
        {
            H_OUT(fmcwtxrx);
        }
    }
    
    //GroupCode'10' is TX.DEV(?)
    H_OUT(fmcwtxrx);
    L_OUT(fmcwtxrx);
    
    //STB Signal
    STBOUT(fmcwtxrx);
}


/*
 * �y�����@�̃��t�@�����X�J�E���^��ݒ肷��z
 *  1. ��������ǂݍ��񂾃��t�@�����X�J�E���^��2�i���ɕϊ��i�z��Ƃ��Ċi�[�j
 *  2. �i�[����2�i���ɍ��킹��High��Low�𖳐��@�ɑ���i�ݒ�̊́j
 *  3. �O���[�v�R�[�h�𑗂�'11'
 *  4. STB�M���𑗂�
 */
void RFDOUT(int fmcwtxrx, int Nref){
    int Nref_b[12];
    
    for(int i=0; i<12; i++){
        Nref_b[i] = 0;
    }
    
    //Nref transforms decimal to binary
    for(int i=0; Nref>0; i++){
        Nref_b[i] = Nref % 2;
        Nref = Nref / 2;
    }
    
    //Send Nref data(binay) to communication module
    for (int i=0; i<12; i++)
    {
        if(Nref_b[i] == 0)
        {
            L_OUT(fmcwtxrx);
        }
        if(Nref_b[i] == 1)
        {
            H_OUT(fmcwtxrx);
        }
    }
    
    //GroupCode'11' is REF.DEV
    H_OUT(fmcwtxrx);
    H_OUT(fmcwtxrx);
    
    //STB Signal
    STBOUT(fmcwtxrx);
}


/*
 * �y�����@�̃I�v�V�������W�X�^��ݒ肷��i����PLL�ݒ�j�z
 *  1. (T1, T2, T3, CpT1, CpT2, Cpr1, Cpr2, LD1, LD2, Tx, Rx) = (0,0,0,1,1,0,0,0,0,0,1)�𑗂�
 *  2. �O���[�v�R�[�h�𑗂�'00'
 *  3. STB�M���𑗂�
 */
void OPINIT(int fmcwtxrx){
    //Send PLL Common DataSet to communiction module
    L_OUT(fmcwtxrx);//T1
    L_OUT(fmcwtxrx);//T2
    L_OUT(fmcwtxrx);//T3
    H_OUT(fmcwtxrx);//CpT1
    H_OUT(fmcwtxrx);//CpT2
    L_OUT(fmcwtxrx);//Cpr1
    L_OUT(fmcwtxrx);//Cpr2
    L_OUT(fmcwtxrx);//LD1
    L_OUT(fmcwtxrx);//LD2
    L_OUT(fmcwtxrx);//Tx
    H_OUT(fmcwtxrx);//Rx
    
    //GroupCode'00' is option reg.
    L_OUT(fmcwtxrx);
    L_OUT(fmcwtxrx);
    
    //STB Signal
    STBOUT(fmcwtxrx);
}


/*
 * �yFMTX��PLL�ݒ���s���z
 *  1. �I�v�V�������W�X�^�̐ݒ�
 *  2. ���t�@�����X�J�E���^�̐ݒ�
 *  3. �v���O���}�u���J�E���^�̐ݒ�
 */
void FMTX(int Nref, int *Nprg){
    int fmtx = FMTX_;
    OPINIT(fmtx);
    RFDOUT(fmtx, Nref);
    OUTFQ(fmtx, Nprg);
}


/*
 * �yCWTX��PLL�ݒ���s���z
 *  1. �I�v�V�������W�X�^�̐ݒ�
 *  2. ���t�@�����X�J�E���^�̐ݒ�
 *  3. �v���O���}�u���J�E���^�̐ݒ�
 */
void CWTX(int Nref, int *Nprg){
    int cwtx = CWTX_;
    OPINIT(cwtx);
    RFDOUT(cwtx, Nref);
    OUTFQ(cwtx, Nprg);
}


/*
 * �yFMRX��PLL�ݒ���s���z
 *  1. �I�v�V�������W�X�^�̐ݒ�
 *  2. ���t�@�����X�J�E���^�̐ݒ�
 *  3. �v���O���}�u���J�E���^�̐ݒ�
 */
void FMRX(int Nref, int *Nprg){
    int fmrx = FMRX_;
    OPINIT(fmrx);
    RFDOUT(fmrx, Nref);
    OUTFQ(fmrx, Nprg);
}


/*
 * �yPLL�ݒ���s���z
 */
void SetPLL(int FMTX_Nref, int FMTX_Nprg, int CWTX_Nref, int CWTX_Nprg, int FMRX_Nref, int FMRX_Nprg){
    FMTX(FMTX_Nref, FMTX_Nprg);
    CWTX(CWTX_Nref, CWTX_Nprg);
    FMRX(FMRX_Nref, FMRX_Nprg);
}



/*
 * �y���[���X�M����'V'�𑗂�z
 *  1. CWKEY�[�q��0��1��0�ƕω�������
 *  2. ��1.���v�R��s��
 */
void Morse_V(void){
    CWTX_KEY = 1;
    __delay_ms(50);
    CWTX_KEY = 0;
    __delay_ms(50);

    CWTX_KEY = 1;
    __delay_ms(50);
    CWTX_KEY = 0;
    __delay_ms(50);

    CWTX_KEY = 1;
    __delay_ms(50);
    CWTX_KEY = 0;
    __delay_ms(50);

    CWTX_KEY = 1;
    __delay_ms(150);
    CWTX_KEY = 0;
    __delay_ms(50);
}


/*
 * �y�����������s��Ȃ��i�ҋ@�j�z
 *  5�������ҋ@����
 */
void _NOP(void) {
    for(int i=0; i<5; i++){
        NOP();
    }
}