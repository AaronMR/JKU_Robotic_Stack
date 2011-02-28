
class structType_S {
public:
    virtual char *serialize(char* maki) = 0;
    virtual char *Unserialize(char* maki) = 0;
    virtual void iniSHM(int shm_in, int shm_out, char* SHM_name) = 0;
    virtual int printStruct(char* data2print) = 0;
    void storeData();
};

class struct_comStruc : public structType_S {
public:
    struct_comStruc();

    char* serialize(char* maki);
    char* Unserialize(char* maki);
    void storeData(Joy *joy);
    bool haveSubscriber;
    bool havePublisher;

    void iniSHM(int shm_in, int shm_out, char* SHM_name);
    int printStruct(char* data2print);

    // data to send and receive
    comStruc auxSerialize;
    comStruc auxUnSerialize;

    // struct to share data with the SHM
    struct comStruc *dataIN;
	struct comStruc *dataOUT;
	//float pause ;
	//float t;


};

class struct_Joy : public structType_S {
public:
    struct_Joy();

    char* serialize(char* maki);
    char* Unserialize(char* maki);
    void storeData(Joy *joy);

    void iniSHM(int shm_in, int shm_out, char* SHM_name);
    int printStruct(char* data2print);

    // data to send and receive
    Joy auxSerialize;
    Joy auxUnSerialize;

    // struct to share data with the SHM
    struct Joy *dataIN;
	struct Joy *dataOUT;

};

class struct_Twist : public structType_S {
public:
    struct_Twist();
    char* serialize(char* maki);
    char* Unserialize(char* maki);
    void storeData(Joy *joy);

    void iniSHM(int shm_in, int shm_out, char* SHM_name);
    int printStruct(char* data2print);

    // data to send and receive
    Twist auxSerialize;
    Twist auxUnSerialize;

    // struct to share data with the SHM
    struct Twist *dataIN;
	struct Twist *dataOUT;

};

class struct_Pose : public structType_S {
public:
    struct_Pose();
    char* serialize(char* maki);
    char* Unserialize(char* maki);
    void storeData(Joy *joy);
    //Joy auxJoy1;

    void iniSHM(int shm_in, int shm_out, char* SHM_name);
    int printStruct(char* data2print);

    // data to send and receive
    Pose auxSerialize;
    Pose auxUnSerialize;

    // struct to share data with the SHM
    struct Pose *dataIN;
	struct Pose *dataOUT;

};

class struct_posWheels : public structType_S {
public:
    struct_posWheels();

    char* serialize(char* data2s);
    char* Unserialize(char* data2us);

    void iniSHM(int shm_in, int shm_out, char* SHM_name);
    int printStruct(char* data2print);

    // data to send and receive
    posWheels_t auxSerialize;
    posWheels_t auxUnSerialize;

    // struct to share data with the SHM
    struct posWheels_t *dataIN;
	struct posWheels_t *dataOUT;

};

class struct_Odometry : public structType_S {
public:
    struct_Odometry();
    char* serialize(char* data2s);
    char* Unserialize(char* data2us);

    void iniSHM(int shm_in, int shm_out, char* SHM_name);
    int printStruct(char* data2print);

    // data to send and receive
    odometry_t auxSerialize;
    odometry_t auxUnSerialize;

    // struct to share data with the SHM
    struct odometry_t *dataIN;
	struct odometry_t *dataOUT;

};

