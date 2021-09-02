
//
//	hspvar.cpp header
//
#ifndef __hspvar_core_h
#define __hspvar_core_h

#define HSPVAR_FLAG_NONE 0
#define HSPVAR_FLAG_LABEL 1
#define HSPVAR_FLAG_STR 2
#define HSPVAR_FLAG_DOUBLE 3
#define HSPVAR_FLAG_INT 4
#define HSPVAR_FLAG_STRUCT 5
#define HSPVAR_FLAG_COMSTRUCT 6

//	7��VARIANT�ŗ\��ς�

#define HSPVAR_FLAG_USERDEF 8
#define HSPVAR_FLAG_MAX 8

#define HSPVAR_MODE_NONE -1
#define HSPVAR_MODE_MALLOC 1
#define HSPVAR_MODE_CLONE 2

#define HSPVAR_ERROR_INVALID HSPERR_WRONG_EXPRESSION
#define HSPVAR_ERROR_DIVZERO HSPERR_DIVIDED_BY_ZERO
#define HSPVAR_ERROR_TYPEMISS HSPERR_TYPE_MISMATCH
#define HSPVAR_ERROR_ARRAYOVER HSPERR_ARRAY_OVERFLOW
#define HSPVAR_ERROR_ILLEGALPRM HSPERR_ILLEGAL_FUNCTION

#define HSPVAR_SUPPORT_STORAGE 1				// �Œ蒷�X�g���[�W�T�|�[�g
#define HSPVAR_SUPPORT_FLEXSTORAGE 2			// �ϒ��X�g���[�W�T�|�[�g
#define HSPVAR_SUPPORT_FIXEDARRAY 4				// �z��T�|�[�g
#define HSPVAR_SUPPORT_FLEXARRAY 8				// �ϒ��z��T�|�[�g
#define HSPVAR_SUPPORT_ARRAYOBJ 16				// �A�z�z��T�|�[�g
#define HSPVAR_SUPPORT_FLEXSIZE 32				// �v�f���Ƃ̃f�[�^���ϒ�
#define HSPVAR_SUPPORT_NOCONVERT 64				// ������̌^�ϊ��𖳌��ɂ���
#define HSPVAR_SUPPORT_VARUSE 128				// varuse�֐��̃`�F�b�N��L���ɂ���
#define HSPVAR_SUPPORT_TEMPVAR 256				// �e���|�����ϐ��Ƃ��Ďg�p����
#define HSPVAR_SUPPORT_USER1 0x4000				// ���[�U�[�t���O1
#define HSPVAR_SUPPORT_USER2 0x8000				// ���[�U�[�t���O2

#define HSPVAR_SUPPORT_MISCTYPE (HSPVAR_SUPPORT_ARRAYOBJ)

typedef void * PDAT;							// �f�[�^�̎��Ԃւ̃|�C���^
typedef int APTR;								// �z��f�[�^�ւ̃I�t�Z�b�g�l

enum
{
CALCCODE_ADD = 0,
CALCCODE_SUB,
CALCCODE_MUL,
CALCCODE_DIV,
CALCCODE_MOD,
CALCCODE_AND,
CALCCODE_OR,
CALCCODE_XOR,
CALCCODE_EQ,
CALCCODE_NE,
CALCCODE_GT,
CALCCODE_LT,
CALCCODE_GTEQ,
CALCCODE_LTEQ,
CALCCODE_RR,
CALCCODE_LR,
CALCCODE_MAX
};


//	PVAL structure
//
typedef struct
{
	//	Memory Data structure (2.5 compatible)
	//
	short	flag;		// type of val
	short	mode;		// mode (0=normal/1=clone/2=alloced)
	int		len[5];		// length of array 4byte align (dim)
	int		size;		// size of Val
	char	*pt;		// ptr to array

	//	Memory Data structure (3.0 compatible)
	//
	void	*master;			// Master pointer for data
	unsigned short	support;	// Support Flag
	short	arraycnt;			// Array Set Count
	int		offset;				// Array Data Offset
	int		arraymul;			// Array Multiple Value 
} PVal;


//		command execute core function
//
typedef struct
{
	//		�f�[�^�t�B�[���h
	//
	short flag;							// �^�^�C�v�l (�e�A�v���P�[�V�����Őݒ肳��܂�)
	short aftertype;					// ���Z��̃^�C�v�l
	short version;						// �^�^�C�v�����^�C���o�[�W����(0x100 = 1.0)
	unsigned short support;				// �T�|�[�g�󋵃t���O(HSPVAR_SUPPORT_*)
	short basesize;						// �P�̃f�[�^���g�p����T�C�Y(byte) / �ϒ��̎���-1
	short opt;							// (���g�p)

	char *vartype_name;					// �^�^�C�v��������ւ̃|�C���^
	char *user;							// ���[�U�[�f�[�^(���g�p)

	//		�V�X�e���Q�ƁE�^�ϊ��p
	//
	void *(*Cnv)( const void *buffer, int flag );
	void *(*CnvCustom)( const void *buffer, int flag );
	PDAT *(*GetPtr)( PVal *pval );

	void *(*ArrayObjectRead)( PVal *pval, int *mptype );// �z��v�f�̎w�� (�A�z�z��/�ǂݏo��)
	void (*ArrayObject)( PVal *pval );							// �z��v�f�̎w�� (�A�z�z��/�������ݏ���)
	void (*ObjectWrite)( PVal *pval, void *data, int type );		// HSPVAR_SUPPORT_NOCONVERT�w�莞�̑��
	void (*ObjectMethod)( PVal *pval );							// �ϐ��ɑ΂��郁�\�b�h�̎w��

	void (*Alloc)( PVal *pval, const PVal *pval2 );		// �ϐ����������m�ۂ���
	void (*Free)( PVal *pval );						// �ϐ����������������

	int (*GetSize)( const PDAT *pdat );			// �v�f���g�p���郁�����T�C�Y��Ԃ�(�ϒ��̂���)
	int (*GetUsing)( const PDAT *pdat );			// �v�f���g�p���ł��邩��Ԃ�(varuse�֐��p)

	//		�ϐ��o�b�t�@(�o�C�i��)�̃|�C���^�ƃT�C�Y��Ԃ�
	//		(�v�f���ϒ�(str)�̏ꍇ�͊Y������P�z��o�C�i���̂�)
	//		(�v�f���Œ蒷(int,double)�̏ꍇ�͑S�z��o�C�i��)
	//		(�T�C�Y�̓������m�ۃT�C�Y��Ԃ�)
	void *(*GetBlockSize)( PVal *pval, PDAT *pdat, int *size );

	//		�o�C�i���f�[�^�p�Ƀ������u���b�N���m�ۂ���
	//		(�v�f���ϒ�(str)�̏ꍇ�Ƀu���b�N�T�C�Y�������I�Ɋm�ۂ���)
	//		(�Œ蒷�̏ꍇ�͉������Ȃ�)
	void (*AllocBlock)( PVal *pval, PDAT *pdat, int size );

	//		����p�֐�(�^�̈�v���ۏႳ��܂�)
	//
	void (*Set)( PVal *pval, PDAT *pdat, const void *in );

	//		���Z�p�֐�(�^�̈�v���ۏႳ��܂�)
	//
	void (*AddI)( PDAT *pval, const void *val );
	void (*SubI)( PDAT *pval, const void *val );
	void (*MulI)( PDAT *pval, const void *val );
	void (*DivI)( PDAT *pval, const void *val );
	void (*ModI)( PDAT *pval, const void *val );

	void (*AndI)( PDAT *pval, const void *val );
	void (*OrI)( PDAT *pval, const void *val );
	void (*XorI)( PDAT *pval, const void *val );

	void (*EqI)( PDAT *pval, const void *val );
	void (*NeI)( PDAT *pval, const void *val );
	void (*GtI)( PDAT *pval, const void *val );
	void (*LtI)( PDAT *pval, const void *val );
	void (*GtEqI)( PDAT *pval, const void *val );
	void (*LtEqI)( PDAT *pval, const void *val );

	void (*RrI)( PDAT *pval, const void *val );
	void (*LrI)( PDAT *pval, const void *val );
} HspVarProc;

extern HspVarProc *hspvarproc;
extern int hspvartype_max;

typedef void (* HSPVAR_COREFUNC) (HspVarProc *);

extern PVal *mem_pval;
#define HspVarCoreGetPVal( flag ) (&mem_pval[flag])


//	flex value define
//
#define FLEXVAL_TYPE_NONE 0
#define FLEXVAL_TYPE_ALLOC 1
#define FLEXVAL_TYPE_CLONE 2
typedef struct
{
	short type;			// typeID
	short myid;			// �ŗLID(���g�p)
	short customid;		// structure ID
	short clonetype;	// typeID for clone
	int size;			// data size
	void *ptr;			// data ptr
} FlexValue;


/*
	typefunc

	��{�^�C�v HSPVAR_FLAG_STR �` HSPVAR_FLAG_DOUBLE
	�g���^�C�v HSPVAR_FLAG_USERDEF �ȍ~

	���̕]����pval->pt���Q�Ƃ��邽�߁A��ɔz��0�̃|�C���g��pval->pt���w�������K�v������B
*/


//		Core System Main Function
//
void HspVarCoreInit( void );
void HspVarCoreBye( void );
void HspVarCoreResetVartype( int expand );
int HspVarCoreAddType();
void HspVarCoreRegisterType( int flag, HSPVAR_COREFUNC func );
HspVarProc *HspVarCoreSeekProc( const char *name );

//		low level support functions
//
void HspVarCoreDup( PVal *pval, PVal *arg, APTR aptr );
void HspVarCoreDupPtr( PVal *pval, int flag, void *ptr, int size );
void HspVarCoreClear( PVal *pval, int flag );
void HspVarCoreClearTemp( PVal *pval, int flag );
void HspVarCoreDim( PVal *pval, int flag, int len1, int len2, int len3, int len4 );
void HspVarCoreDimFlex( PVal *pval, int flag, int len0, int len1, int len2, int len3, int len4 );
void HspVarCoreReDim( PVal *pval, int lenid, int len );
void *HspVarCoreCnvPtr( PVal *pval, int flag );
PDAT *HspVarCorePtrAPTR( PVal *pv, APTR ofs );
void HspVarCoreArray( PVal *pval, int offset );

//		macro for PVal
//
#define HspVarCoreGetProc( flag ) (&hspvarproc[flag])
#define HspVarCoreDispose( pv ) hspvarproc[(pv)->flag].Free(pv)
#define HspVarCoreReset( pv ) ((pv)->offset=0,(pv)->arraycnt=0)
#define HspVarCorePtr( pv ) (hspvarproc[(pv)->flag].GetPtr(pv))
#define HspVarCoreArrayObject( pv,in ) (hspvarproc[(pv)->flag].ArrayObject(pv,in))	//	�z��̗v�f���w�肷��(�ŏ���Reset���Ă�ł�������)

#define HspVarCoreSet( pv,in ) hspvarproc[(pv)->flag].Set( pv, in )
#define HspVarCoreCnv( in1,in2,in3 ) hspvarproc[in2].Cnv( in3,in1 )		// in1->in2�̌^��in3�|�C���^��ϊ�����

#define HspVarCoreGetBlockSize( pv,in1,out ) hspvarproc[(pv)->flag].GetBlockSize( pv,in1,out )
#define HspVarCoreAllocBlock( pv,in1,in2 ) hspvarproc[(pv)->flag].AllocBlock( pv,in1,in2 )
#define HspVarCoreGetUsing( pv,in1 ) hspvarproc[(pv)->flag].GetUsing( in1 )

#define HspVarCoreGetAPTR( pv ) ((pv)->offset)

#define HspVarCoreCopyArrayInfo( pv, src ) (pv)->arraycnt = (src)->arraycnt;(pv)->offset = (src)->offset;(pv)->arraymul = (src)->arraymul;

#endif
