#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include <time.h>
#define maks_admin 5
#define maks_pinjam 4

//klo pertama download -> aktivasi dengan kode kelompok7

//saat pertama kali didownload dan dijalankan, masukan kode kelompok7 untuk mengaktivasi program
//lengkapi data perpustakaan
//mulai jalankan program
//beberapa buku telah dimasukan
//peminjaman buku 1 bulan (misal pinjam 1 jan kembali 1 feb) -> mempermudah perhitungan
//

//ID anggota 5 digit
//ID admin 4 digit
//ID buku 6 digit

//deklarasi beberapa function
//main function berada paling bawah
void menu_admin();
void menu_utama();
void menu_anggota();
void login_admin();
void menu_awal();
void isifiletxt(char namafile[40]);
void load(int num);
void database_anggota();
void database_buku();
void pinjam_kembali();
//void hitungdenda(struct today pinjam, struct today kembali);

//struct
struct today{
    int tanggal;
    int bulan;
    int tahun;
    int detik;
    int menit;
    int jam;
};

struct dataadmin{
    int ID;
    char nama[40];
};

struct databuku{
    int ID;
    char judul[50];
    char penulis[50];
    char kategori[20];
    char penerbit[40];
    int tahun;
    int tersedia;
};

struct datapinjam{
    int no;
    struct databuku buku;
    struct today w_pinjam;
    struct today w_kembali;
    int status;
};

struct pengunjung{
    int ID;
    char nama[40];
    int jumlahpinjam;
};

struct dataperpus{
    char nama[50];
    char alamat[100];
    int jumlahadmin;
    struct dataadmin petugas[maks_admin];
    char passadmin[20];
};

//var global
struct today waktu;

//function
void load(int x){
    printf("Memproses");
    while (x > 0){
        Sleep(1000); 
        printf(". ");
        x--;
    }
    printf("\n");
}

void current_time(struct today *waktu){
    time_t now;
    now = time(NULL);
    struct tm *local;
    local = localtime(&now); 
    waktu->tanggal = local->tm_mday;
    waktu->bulan = local->tm_mon + 1;
    waktu->tahun = local->tm_year + 1900;
    waktu->detik = local->tm_sec;
    waktu->menit = local->tm_min;
    waktu->jam = local->tm_hour;
}

void header(struct today waktu){
    struct dataperpus perpus;
    int i = 0;
    FILE *fperpus = fopen("dataperpus_b.bin", "rb");
    while(fread(&perpus, sizeof(struct dataperpus), 1, fperpus)){
        printf("%s\n", perpus.nama);
        printf("%s\n", perpus.alamat);
    }
    fclose(fperpus);
    current_time(&waktu);
    printf("------------------------------------------\n");
    printf("%d:%d:%d                          %d-%d-%d\n", waktu.jam, waktu.menit, waktu.detik, waktu.tanggal, waktu.bulan, waktu.tahun);
    printf("------------------------------------------\n");
}

int ganti_mode(){
    int mode;
    system("cls");
    printf("[1] Dark Mode\n");
    printf("[2] Light Mode\n");
    printf("Pilih Mode : ");
    scanf("%d", &mode);
    load(3);
    return mode;
}

void isifiletxt(char namafile[40]){
    system("cls");
    FILE *f;
    char buff[100];
    f = fopen(namafile, "r");
    while(fgets(buff, sizeof(buff), f)){
        printf("%s", buff);
    }
    fclose(f);
    system("pause");
}

int IDgenerator(int digit){
    int up = pow(10, digit) - 1;
    int low = pow(10, digit-1);
    return (rand()%(up-low+1) + low);
}

int IDcheck(int num, int idarr[num], int ID, int digit){
    int help = 0;                            // a    b    c    d
    if (num>1){                             // 1041 2098 3650  2001
        while(help<num){                    // 1041x 3650x 1041x 2001
            while(ID == idarr[help]){       
                ID = IDgenerator(digit);
                help = -1;
            }
            help++;
        }
    }
    return ID;
}

int cekfile(char namafile[20]){
    FILE *f;
    f = fopen(namafile, "r");
    if(f == NULL){
        return 0;
    }
    fclose(f);
    return 1;
}

int datecheck(int x, int y, int z){
    int bulan[12][2] = {{31,31}, 
                        {28,29}, 
                        {31,31}, 
                        {30,30}, 
                        {31,31},  
                        {30,30},   
                        {31,31},
                        {31,31},
                        {30,30},
                        {31,31},
                        {30,30},
                        {31,31}};
    if (z%400==0||z%100!=0&&z%4==0){
        z = 1;
    }else{
        z = 0;
    }
    return (x <= bulan[y-1][z]); 
}

int cekbuku(char judul[40], int num){
    //num 1 -> cek berdasarkan kategori
    //num 2 -> cek berdasarkan judul
    //num else -> cek berdasarkan ID
    struct databuku buku;
    FILE *f = fopen("daftarbuku.bin", "rb");
    int cek = 0;
    while(fread(&buku, sizeof(struct databuku),1, f)){
        if(num == 1){
            strcmpi(judul, buku.kategori) == 0 ? (cek += 1) : (cek += 0);
        }else if(num==2){
            strcmpi(judul, buku.judul) == 0 ? (cek+=1) : (cek+=0);
        }else{
            (buku.ID == num) ? (cek += 1) : (cek+=0);
        }
    }
    fclose(f);
    return cek;
}

int cekanggota(char nama[40], int num){
    //num 1 -> cek berdasarkan nama
    //num else -> cek berdasarkan ID
    struct pengunjung anggota;
    FILE *f = fopen("daftaranggota.bin", "rb");
    int cek = 0;
    while(fread(&anggota, sizeof(struct pengunjung),1, f)){
        if(num == 1){
            strcmpi(nama, anggota.nama) == 0 ? (cek += 1) : (cek += 0);
        }else{
            (anggota.ID == num) ? (cek += 1) : (cek+=0);
        }
    }
    fclose(f);
    return cek;
}

void tambahbuku(){
    struct today waktu;
    struct databuku buku;
    int tempID, i=0, num=0, j = 0;
    system("cls");
    header(waktu);
    FILE *fb = fopen("daftarbuku.bin", "a+"), *f = fopen("daftarbuku.txt", "a+");
    while(fread(&buku, sizeof(struct databuku), 1, fb)){
        num++;
    }
    int id[num];
    fclose(fb);
    fb = fopen("daftarbuku.bin", "rb");
    while(fread(&buku, sizeof(struct databuku), 1, fb)){
        id[i] = buku.ID;
        i++;
    }
    fclose(fb);
    fb = fopen("daftarbuku.bin", "a+");
    buku.ID = IDcheck(num, id, IDgenerator(6), 6);
    //printf("\nnum : %d\ni : %d", num, i);
    printf("Silahkan masukan data buku baru\n");
    printf("Judul Buku       : ");
    scanf("\n");
    scanf("%[^\n]s", buku.judul);
    printf("Penulis          : ");
    scanf("\n");
    scanf("%[^\n]s", buku.penulis);
    printf("Penerbit         : ");
    scanf("\n");
    scanf("%[^\n]s", buku.penerbit);
    printf("Tahun terbit     : ");
    scanf("%d", &buku.tahun);
    printf("Kategori         : ");
    scanf("\n");
    scanf("%[^\n]s", buku.kategori);
    printf("Jumlah           : ");
    scanf("%d", &buku.tersedia);
    fwrite(&buku, sizeof(struct databuku), 1, fb);
    fprintf(f, "%d %-40s %-40s %-40s %-5d %-20s %-3d\n", buku.ID, buku.judul, buku.penulis, buku.penerbit, buku.tahun, buku.kategori, buku.tersedia);
    load(3);
    printf("Buku berhasil ditambahkan!\n");
    system("pause");
    fclose(f);
    fclose(fb);
}

void tambahanggota(){
    system("cls");
    header(waktu);
    struct pengunjung anggota;
    int tempID, i=0, num=0, j = 0;
    FILE *fb = fopen("daftaranggota.bin", "a+"), *f = fopen("daftaranggota.txt", "a+");
    while(fread(&anggota, sizeof(struct pengunjung), 1, fb)){
        num++;
    }
    int id[num];
    fclose(fb);
    fb = fopen("daftaranggota.bin", "rb");
    while(fread(&anggota, sizeof(struct pengunjung), 1, fb)){
        id[i] = anggota.ID;
        i++;
    }
    fclose(fb);
    fb = fopen("daftaranggota.bin", "a+");
    anggota.ID = IDcheck(num, id, IDgenerator(5), 5);
    //printf("\nnum : %d\ni : %d", num, i);
    printf("Silahkan masukan data anggota baru\n");
    printf("Nama            : ");
    scanf("\n");
    scanf("%[^\n]s", anggota.nama);
    anggota.jumlahpinjam = 0;
    fwrite(&anggota, sizeof(struct pengunjung), 1, fb);
    fprintf(f, "%d %-40s\n", anggota.ID, anggota.nama);
    fclose(f);
    fclose(fb);
    load(3);
    printf("Anggota baru berhasil ditambahkan!\n");
    printf("-------------------------------------------------------------------------------------\n");
    printf("Nama    : %s\n", anggota.nama);
    printf("ID      : %d\n", anggota.ID);
    system("pause");
}

void hapus_buku(){
    struct today waktu;
    struct databuku buku;
    char judul[40];
    int flag = 0;
    FILE *temp = fopen("temp_daftarbuku.bin", "a+"), *fb = fopen("daftarbuku.bin", "rb");
    system("cls");
    header(waktu);
    printf("Masukan Judul Buku yang ingin dihapus : ");
    scanf("\n");
    scanf("%[^\n]s", judul);
    while(fread(&buku, sizeof(struct databuku), 1, fb)){
        if(strcmpi(buku.judul, judul) == 0){
            flag = 1;
        }
    }
    fclose(fb);
    fb = fopen("daftarbuku.bin", "a+");
    while(fread(&buku, sizeof(struct databuku), 1, fb)){
        if (strcmpi(buku.judul, judul) != 0){
            fwrite(&buku, sizeof(struct databuku), 1, temp);
        }
    }
    if(flag==1){                             // ****$** <- data lama (x)
        load(3);                             // ****** data baru (y)
        printf("Buku berhasil dihapus\n");   // data lama kita hapus (x)
    }else{                                   // nama dari daru kita ubah jdi nama data lama (y) -> (x)
        printf("Maaf nama yang Anda inginkan tidak terdaftar!\n");
    }
    fclose(fb);
    fclose(temp);
    remove("daftarbuku.bin");
    rename("temp_daftarbuku.bin","daftarbuku.bin");
    system("pause");
}

void edit_buku(){
    struct today waktu;
    struct databuku buku;
    char judul[40];
    int flag = 0;
    FILE *temp = fopen("temp_daftarbuku.bin", "a+"), *fb = fopen("daftarbuku.bin", "rb");
    system("cls");
    header(waktu);
    printf("Masukan Judul Buku yang ingin Anda edit : ");
    scanf("\n");
    scanf("%[^\n]s", judul);
    while(fread(&buku, sizeof(struct databuku), 1, fb)){
        if(strcmpi(buku.judul, judul) == 0){
            flag = 1;
        }
    }
    fclose(fb);
    fb = fopen("daftarbuku.bin", "rb");
    while(fread(&buku, sizeof(struct databuku), 1, fb)){
        if (strcmpi(buku.judul, judul) == 0){
            printf("Judul Buku       : ");
            scanf("\n");
            scanf("%[^\n]s", buku.judul);
            printf("Penulis          : ");
            scanf("\n");
            scanf("%[^\n]s", buku.penulis);
            printf("Penerbit         : ");
            scanf("\n");
            scanf("%[^\n]s", buku.penerbit);
            printf("Tahun terbit     : ");
            scanf("%d", &buku.tahun);
            printf("Kategori         : ");
            scanf("\n");
            scanf("%[^\n]s", buku.kategori);
            printf("Jumlah           : ");
            scanf("%d", &buku.tersedia);
            fwrite(&buku, sizeof(struct databuku), 1, temp);
        } else{
            fwrite(&buku, sizeof(struct databuku), 1, temp);
        }
    }
    if(flag==1){
        load(3);
        printf("Buku berhasil diedit\n");
    }else{
        printf("Maaf nama yang Anda inginkan tidak terdaftar!\n");
    }
    fclose(fb);
    fclose(temp);
    remove("daftarbuku.bin");
    rename("temp_daftarbuku.bin","daftarbuku.bin");
    system("pause");
}

void edit_anggota(){
    struct today waktu;
    struct pengunjung anggota;
    char nama[40];
    int flag = 0;
    FILE *temp = fopen("temp_daftaranggota.bin", "a+"), *fb = fopen("daftaranggota.bin", "rb");
    system("cls");
    header(waktu);
    printf("Masukan Nama Anggota yang ingin diedit : ");
    scanf("\n");
    scanf("%[^\n]s", nama);
    while(fread(&anggota, sizeof(struct pengunjung), 1, fb)){
        if(strcmpi(anggota.nama, nama) == 0){
            flag = 1;
        }
    }
    fclose(fb);
    fb = fopen("daftaranggota.bin", "rb");
    while(fread(&anggota, sizeof(struct pengunjung), 1, fb)){
        if (strcmpi(anggota.nama, nama) == 0){
            printf("Masukan Data Baru\n");
            printf("Nama    : ");
            scanf("\n");
            scanf("%[^\n]s", anggota.nama);
            fwrite(&anggota, sizeof(struct pengunjung), 1, temp);
        } else{
            fwrite(&anggota, sizeof(struct pengunjung), 1, temp);
        }
    }
    if(flag==1){
        load(3);
        printf("Anggota berhasil diedit\n");
    }else{
        printf("Maaf nama yang Anda inginkan tidak terdaftar!\n");
    }
    fclose(fb);
    fclose(temp);
    remove("daftaranggota.bin");
    rename("temp_daftaranggota.bin","daftaranggota.bin");
    system("pause");
}

void hapus_anggota(){
    struct today waktu;
    struct pengunjung anggota;
    char nama[40], namafile[40], format[10]=".bin";
    int flag = 0;
    FILE *temp = fopen("temp_daftaranggota.bin", "a+"), *fb = fopen("daftaranggota.bin", "rb"), *fpinjam;
    system("cls");
    header(waktu);
    printf("Masukan Nama Anggota yang ingin dihapus : ");
    scanf("\n");
    scanf("%[^\n]s", nama);
    while(fread(&anggota, sizeof(struct pengunjung), 1, fb)){
        if(strcmpi(anggota.nama, nama) == 0){
            flag = 1;
            itoa(anggota.ID, namafile,10);
            strcat(namafile,format);
        }
    }
    fclose(fb);
    fb = fopen("daftaranggota.bin", "rb");
    while(fread(&anggota, sizeof(struct pengunjung), 1, fb)){
        if (strcmpi(anggota.nama, nama) != 0){
            fwrite(&anggota, sizeof(struct pengunjung), 1, temp);
        }
    }
    if(flag==1){
        load(3);
        printf("Anggota berhasil dihapus\n");
    }else{
        printf("Maaf nama yang Anda inginkan tidak terdaftar!\n");
    }
    fclose(fb);
    fclose(temp);
    remove("daftaranggota.bin");
    fpinjam=fopen(namafile, "r");
    if(fpinjam!=NULL){
        flag = 10;
    }
    fclose(fpinjam);
    if (flag == 10){
        remove(namafile);
    }
    remove(namafile);
    rename("temp_daftaranggota.bin","daftaranggota.bin");
    system("pause");
}

int display_buku(char judul[40], int num){
    //else -> daftar semua buku
    //1 -> berdasarkan kategori
    //2 -> berdasarkan judul
    FILE *f = fopen("daftarbuku.bin", "rb");
    struct databuku buku;
    if(num!=1&&num!=2){
        system("cls");
        header(waktu);
    }
    if(num==1&&cekbuku(judul, 1)==0||num==2&&cekbuku(judul, 2)==0){
        printf("Maaf, buku yang Anda cari tidak ditemukan!\n");
        //system("pause");
        //fclose(f);
        return 0;
    }else{
        printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("%-8s %-45s %-30s %-30s %-17s %-15s %s\n", "ID", "Judul Buku", "Penulis Buku", "Penerbit", "Kategori", "Tahun Terbit", "Tersedia");
        printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    }
    while(fread(&buku, sizeof(struct databuku), 1, f)){
        switch(num){
            case 1:
                if(strcmpi(judul, buku.kategori)==0){
                    printf("%-8d %-45s %-30s %-30s %-17s %-15d %d\n", buku.ID, buku.judul, buku.penulis, buku.penerbit, buku.kategori, buku.tahun, buku.tersedia);
                }
                break;
            case 2:
                if(strcmpi(judul, buku.judul)==0){
                    printf("%-8d %-45s %-30s %-30s %-17s %-15d %d\n", buku.ID, buku.judul, buku.penulis, buku.penerbit, buku.kategori, buku.tahun, buku.tersedia);
                }
                break;
            default :
                    printf("%-8d %-45s %-30s %-30s %-17s %-15d %d\n", buku.ID, buku.judul, buku.penulis, buku.penerbit, buku.kategori, buku.tahun, buku.tersedia); 
                break;
        }
    }
    fclose(f);
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    system("pause");
}

void display_anggota(char judul[40], int num){
    // 1 -> daftar anggota
    // 2 -. berdasarkan nama
    //else -> berdasarkan ID
    FILE *f = fopen("daftaranggota.bin", "rb"), *fpinjam;
    struct pengunjung anggota;
    struct datapinjam pinjam;
    char namafile[40], format[6]=".bin";
    int flag=0;
    system("cls");
    header(waktu);
    if(num==1){
        system("cls");
        header(waktu);
        printf("\n--------------------------------------------------------------------------------------------------------------\n");
        printf("%-8s %-45s %-30s\n", "ID", "Nama", "Jumlah Buku Dipinjam");
        printf("--------------------------------------------------------------------------------------------------------------\n");
    }else{
        itoa(num, namafile, 10);
        strcat(namafile, format);
        fpinjam = fopen(namafile, "rb");
    }
    while(fread(&anggota, sizeof(struct pengunjung), 1, f)){
        if(num == 1){
            flag = 1;
            printf("%-8d %-45s %-30d\n", anggota.ID, anggota.nama, anggota.jumlahpinjam);
        }else if(num==anggota.ID){
            flag = 1;
            printf("ID                 : %d\n", anggota.ID);
            printf("Nama               : %s\n", anggota.nama);
            printf("Belum dikembalikan : %d\n", anggota.jumlahpinjam);
            if(fpinjam != NULL){
                printf("----------------------------------------------------------------------------------------------\n");
                printf("%-4s %-18s %-40s %-17s %-18s\n", "No", "Tanggal Pinjam", "Judul Buku", "Dikembalikan","Tanggal Kembali");
                printf("----------------------------------------------------------------------------------------------\n");
                while(fread(&pinjam, sizeof(struct datapinjam), 1, fpinjam)){
                    printf("%-4d %-2d/%-2d/%-12d %-40s %-17d %-2d/%-2d/%-12d\n", pinjam.no, pinjam.w_pinjam.tanggal, pinjam.w_pinjam.bulan, pinjam.w_pinjam.tahun, pinjam.buku.judul, pinjam.status, pinjam.w_kembali.tanggal, pinjam.w_kembali.bulan, pinjam.w_kembali.tahun);
                }
                printf("----------------------------------------------------------------------------------------------\n");
                fclose(fpinjam);
            }else{
                printf("-------------------------------------------------------------------------------------\n");
                printf("Tidak ada riwayat peminjaman\n");
            }
        }else if(num==2){
            if(!strcmpi(judul, anggota.nama)){
                printf("ID                 : %d\n", anggota.ID);
                printf("Nama               : %s\n", anggota.nama);
                printf("Belum dikembalikan : %d\n", anggota.jumlahpinjam);
                printf("-------------------------------------------------------------------------------------\n");
                printf("Daftar riwayat reminjaman buku aanggota dapat dilihat dengan memilih menu 4\n");
            }
        }
    }
    if(flag == 0){
        printf("Data yang Anda inginkan tidak ditemukan!");
    }
    system("pause");
    fclose(f);
}

void pinjam(int id){
    header(waktu);
    struct datapinjam pinjam;
    struct databuku buku;
    struct pengunjung anggota;
    char judul[40], namafile[40], format[10]=".bin";
    int flag = 0, flag1 = 0, no = 0;
    itoa(id, namafile, 10);
    strcat(namafile,format);
    FILE *fbuku, *tempbuku, *fpinjam, *nama, *tempnama;
    printf("Masukan judul buku : ");
    scanf("\n");
    scanf("%[^\n]s", judul);
    load(3);
    if(cekbuku(judul, 2)==1){
        fbuku = fopen("daftarbuku.bin", "rb");
        while(fread(&buku, sizeof(struct databuku), 1, fbuku)){
            if(strcmpi(buku.judul, judul)==0){
                buku.tersedia > 0 ? (flag = 1) : (flag = 0);
            }
        }
        fclose(fbuku);
        nama = fopen("daftaranggota.bin","rb");
        while(fread(&anggota, sizeof(struct pengunjung), 1, nama)){
            if(anggota.ID == id){
                anggota.jumlahpinjam >= maks_pinjam ? (flag1 = 0) : (flag1 = 1);
            }
        }
        fclose(nama);
        if(flag1==1&&flag==1){
            tempbuku = fopen("tempbuku.bin", "a+");
            fbuku = fopen("daftarbuku.bin", "rb");
            while(fread(&buku, sizeof(struct databuku), 1, fbuku)){
                if(strcmpi(buku.judul, judul)==0){
                    buku.tersedia-=1;
                    strcpy(pinjam.buku.judul,buku.judul);
                    strcpy(pinjam.buku.penerbit, buku.penerbit);
                    strcpy(pinjam.buku.penulis, buku.penulis);
                    strcpy(pinjam.buku.kategori, buku.kategori);
                    pinjam.buku.ID = buku.ID;
                    pinjam.buku.tahun = buku.tahun;
                }
                fwrite(&buku, sizeof(struct databuku), 1, tempbuku);
            }
            fclose(fbuku);
            fclose(tempbuku);
            nama = fopen("daftaranggota.bin","rb");
            tempnama = fopen("tempanggota.bin", "a+");
            while(fread(&anggota, sizeof(struct pengunjung), 1, nama)){
                if(anggota.ID == id){
                    anggota.jumlahpinjam+=1;
                    fwrite(&anggota, sizeof(struct pengunjung), 1, tempnama);
                }else{
                    fwrite(&anggota, sizeof(struct pengunjung), 1, tempnama);
                }
            }
            fclose(nama);
            fclose(tempnama);
            fpinjam = fopen(namafile, "rb");
            while(fread(&pinjam, sizeof(struct datapinjam), 1, fpinjam)){
                no = pinjam.no;
            }
            fclose(fpinjam);
            fpinjam = fopen(namafile, "a+");
            current_time(&waktu);
            pinjam.no = no+1;
            pinjam.w_pinjam.tanggal = waktu.tanggal;
            pinjam.w_pinjam.bulan = waktu.bulan;
            pinjam.w_pinjam.tahun = waktu.tahun;
            pinjam.w_kembali.tanggal = 0;
            pinjam.w_kembali.bulan = 0;
            pinjam.w_kembali.tahun = 0;
            pinjam.status = 0;
            fwrite(&pinjam, sizeof(struct datapinjam), 1, fpinjam);
            fclose(fpinjam);
            remove("daftarbuku.bin");
            rename("tempbuku.bin", "daftarbuku.bin");
            remove("daftaranggota.bin");
            rename("tempanggota.bin", "daftaranggota.bin");
            printf("Buku berhasil dipinjamkan!\n");
        }else if(flag1 == 0 && flag == 0){
            printf("Buku yang Anda cari sedang tidak tersedia!\n");
            printf("Anda sudah mencapat batas maksmimum buku yang dapat dipinjam!\n");
        }else if (flag1 == 0){
            printf("Anda sudah mencapat batas maksmimum buku yang dapat dipinjam!\n");
        }else if (flag == 0){
            printf("Buku yang Anda cari sedang tidak tersedia!\n");
        }
    }else{
        printf("Maaf buku yang Anda cari tidak terdaftar!\n");
    }
    system("pause");
}

void kembali(int id){
    struct databuku buku;
    struct pengunjung anggota;
    struct datapinjam pinjam;
    struct today dpinjam, waktu2;
    FILE *fbuku, *tempbuku, *fanggota, *tempanggota, *fpinjam, *temppinjam;
    char judul[40], namafile[40], format[10]=".bin";
    int flag = 0, flag1 = 0, no = 0, idbuku, tgl=0, bln=0, thn=0, aman = 0;
    itoa(id, namafile, 10);
    strcat(namafile,format);
    fpinjam = fopen(namafile, "rb");
    printf("----------------------------------------------------------------------------------------------\n");
    printf("%-4s %-18s %-40s %-17s %-18s\n", "No", "Tanggal Pinjam", "Judul Buku", "Dikembalikan","Tanggal Kembali", "Denda");
    printf("----------------------------------------------------------------------------------------------\n");
    while(fread(&pinjam, sizeof(struct datapinjam), 1, fpinjam)){
        printf("%-4d %-2d/%-2d/%-12d %-40s %-17d %-2d/%-2d/%-12d\n", pinjam.no, pinjam.w_pinjam.tanggal, pinjam.w_pinjam.bulan, pinjam.w_pinjam.tahun, pinjam.buku.judul, pinjam.status, pinjam.w_kembali.tanggal, pinjam.w_kembali.bulan, pinjam.w_kembali.tahun);
    }
    fclose(fpinjam);
    printf("Masukan no buku yang akan dikembalikan : ");
    scanf("%d", &no);
    load(3);
    fpinjam = fopen(namafile, "a+");
    temppinjam = fopen("temppinjam.bin", "a+");
    while(fread(&pinjam, sizeof(struct datapinjam), 1, fpinjam)){
        if(pinjam.no == no&& pinjam.status==0){
            aman = 1;
            idbuku = pinjam.buku.ID;
            /*
            pinjam.w_kembali.tanggal = waktu2.tanggal;
            pinjam.w_kembali.bulan = waktu2.bulan;       //ini pengembalian berdasarkan waktu lokal
            pinjam.w_kembali.tahun = waktu2.tahun;       //agar dapat melihat fitur denda, kami tidak menggunakan pengambilan waktu lokal
            */                                           //sehingga tanggal kembali diinput langsung oleh admin
            dpinjam.tanggal = pinjam.w_pinjam.tanggal;   
            dpinjam.bulan = pinjam.w_pinjam.bulan;
            dpinjam.tahun = pinjam.w_pinjam.tahun; 
            do{
                printf("Masukan Waktu Pengembalian\n");
                printf("Tanggal (dd)   : ");
                scanf("%d", &tgl);
                printf("Bulan   (mm)   : ");
                scanf("%d", &bln);
                printf("Tahun   (yyyy) : ");
                scanf("%d", &thn);
            }while(datecheck(tgl,bln,thn)==0);
            pinjam.w_kembali.tanggal = tgl;
            pinjam.w_kembali.bulan = bln;
            pinjam.w_kembali.tahun = thn;
            waktu2.tanggal = tgl;
            waktu2.bulan = bln;
            waktu2.tahun = thn;
            pinjam.status = 1; 
            printf("Buku berhasil dikembalikan!\n");
        }
        fwrite(&pinjam, sizeof(struct datapinjam), 1, temppinjam);
    }
    fclose(fpinjam);
    fclose(temppinjam);
    remove(namafile);
    rename("temppinjam.bin", namafile);
    if(aman==1){
        fbuku = fopen("daftarbuku.bin", "rb");
        tempbuku = fopen("tempbuku.bin", "a+");
        while(fread(&buku, sizeof(struct databuku), 1, fbuku)){
            if(buku.ID == idbuku){
                buku.tersedia+=1;
            }
            fwrite(&buku, sizeof(struct databuku), 1, tempbuku);
        }
        fclose(fbuku);
        fclose(tempbuku);
        remove("daftarbuku.bin");
        rename("tempbuku.bin", "daftarbuku.bin");
        fanggota = fopen("daftaranggota.bin", "rb");
        tempanggota = fopen("tempanggota.bin", "a+");
        while(fread(&anggota, sizeof(struct pengunjung), 1, fanggota)){
            if(id == anggota.ID){
                anggota.jumlahpinjam-=1;
            }
            fwrite(&anggota, sizeof(struct pengunjung), 1, tempanggota);
        }
        fclose(tempanggota);
        fclose(fanggota);
        remove("daftaranggota.bin");
        rename("tempanggota.bin", "daftaranggota.bin");
    }
    system("pause");
}

void aktivasi(){
    char ch, pass[10];
    int i=0, idterpakai[10], help;
    struct dataperpus perpus;
    FILE *f, *fb;
    fb = fopen("dataperpus_b.bin", "a+"); // w r 
    f = fopen("dataperpus.txt", "a+");
    printf("Selamat Datang di Program Manajemen Perpustakaan!\n");
    printf("Silahkan lakukan aktivasi program terlebih dahulu sebelum mulai menggunakan program\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("Masukan password aktivasi : ");
    while((ch=getch())!='\r'){
        putch('*'); 
        pass[i++]=ch;
    }
    pass[i]='\0';  //*****
    while(strcmp(pass, "kelompok7")!=0){
            i = 0;
            printf("\nMaaf password yang Anda masukan salah!\n");
            printf("Masukan password Anda : ");
            while((ch=getch())!='\r'){
                putch('*');
                pass[i++]=ch;
            }
    }
    printf("\n------------------------------------------------------------------------------------\n");
    printf("Nama Perpustakaan     : ");
    scanf("\n");
    scanf("%[^\n]s", perpus.nama);
    printf("Alamat                : ");
    scanf("\n");
    scanf("%[^\n]s", perpus.alamat);
    fprintf(f, "Nama     : %s\nAlamat   : %s\nData Admin\n", perpus.nama, perpus.alamat);
    printf("Jumlah petugas        : ");
    scanf("%d", &perpus.jumlahadmin);
    while(perpus.jumlahadmin>maks_admin||perpus.jumlahadmin<0){
        printf("Jumlah maksimum petugas adalah %d.\nMohon masukan ulang jumlah petugas!\n");
        printf("Jumlah petugas        : ");
        scanf("%d", &perpus.jumlahadmin);
    }
    for (i=0;i<perpus.jumlahadmin;i++){
        printf("Nama petugas ke-%d     : ", i+1);
        scanf("\n");
        scanf("%[^\n]s", perpus.petugas[i].nama);
        perpus.petugas[i].ID = IDgenerator(4);
        if (i>0){
            help = 0;
            while(help<=i){
                while(idterpakai[help] == perpus.petugas[i].ID){
                    perpus.petugas[i].ID = IDgenerator(4);
                    help = -1;
                }
                help++;
            }
            idterpakai[i] = perpus.petugas[i].ID;
        }
        fprintf(f, "%-15s %d \n", perpus.petugas[i].nama, perpus.petugas[i].ID);
    }
    printf("Password tanpa spasi!\n");
    printf("Password Administrator : ");
    scanf("%s", perpus.passadmin);
    fprintf(f, "Password : %s\n", perpus.passadmin);
    fwrite(&perpus, sizeof(struct dataperpus),1, fb);
    printf("------------------------------------------------------------------------------------\n");
    printf(">>                                DATA PERPUSTAKAAN                               <<\n");
    printf("------------------------------------------------------------------------------------\n");
    printf("Nama Perpustakaan     : %s\n", perpus.nama);
    printf("Alamat                : %s\n", perpus.alamat);
    printf("\nData Admin\n");
    for(i=0;i<perpus.jumlahadmin;i++){
        printf("Admin ke-%d           : %s\n", i+1, perpus.petugas[i].nama);
        printf("ID                    : %d\n", perpus.petugas[i].ID);
    }
    printf("Paswrod Administrator : %s\n", perpus.passadmin);
    load(4);
    printf("Program Perpustakaan Anda berhasil diaktifkan!\n");
    printf("ID dan passwrod petugas akan digunakan untuk login pada menu masuk sebagai petugas\n");
    system("pause");
    fclose(f);
    fclose(fb);
}

void login_admin(){
    struct dataperpus perpus;
    system("cls");
    int ID;
    char pass[20], ch;
    int i = 0, j = 0, flag = 0;
    struct today waktu;
    FILE *f = fopen("dataperpus_b.bin", "rb");
    header(waktu);
    printf("Masukan ID Anda       : ");
    scanf("%d", &ID);
    printf("Masukan password Anda : ");
    while((ch=getch())!='\r'&&i<20){
        putch('*');
        pass[i++]=ch;
    }
    pass[i]='\0';
    printf("\n");
    load(3);
    fread(&perpus, sizeof(struct dataperpus), 1, f);
    for(i=0;i<perpus.jumlahadmin;i++){
        if(ID==perpus.petugas[i].ID){
            flag = 1;
            break;
        }
    }
    if (flag!=1){
        fclose(f);
        printf("ID yang Anda masukan salah!\nHarap masukan kembali ID dan Password dengan tepat!\n");
        Sleep(3000);
        login_admin(waktu);
    }
    else{
        while(strcmp(pass, perpus.passadmin)!=0){
            i = 0;
            printf("Maaf password yang Anda masukan salah!\n");
            printf("Masukan password Anda : ");
            while((ch=getch())!='\r'&&i<20){
                putch('*');
                pass[i++]=ch;
            }
            pass[i]='\0';
            printf("\n");
        }
        printf("Login berhasil!\n");
        system("pause");
    }
    fclose(f);
}

//function menu
void menu_awal(){
    if (cekfile("dataperpus.txt") == 0){
        aktivasi();
    }
    system("cls");
    int menu, mode;
    printf("------------------------------------------\n");
    printf(">>    PROGRAM MANAJEMEN PERPUSTAKAAN    <<\n");
    printf("------------------------------------------\n");
    printf("[1] Mulai Program\n");
    printf("[2] Petunjuk Penggunaan Program\n");
    printf("[3] Tentang Kami\n");
    printf("[4] Ganti mode program\n");
    printf("[5] Tutup Program\n");
    printf("Pilih Menu : ");
    scanf("%d", &menu);
    if (menu == 1){
        menu_utama();
        menu_awal();
    }
    else if (menu == 2){
        isifiletxt("petunjuk.txt");
        menu_awal();
    }
    else if (menu == 3){
        isifiletxt("tentang.txt");
        menu_awal();
    }
    else if(menu == 4){
        mode = ganti_mode();
        if (mode == 1){
            system("color 07"); //text white, foreground black
        }
        else if (mode == 2){
            system("color 70"); //text black, foregraound white
        }
        else{
            printf("Maaf, untuk saat ini mode yang Anda pilih belum tersedia!");
            Sleep(2000);
        }
        menu_awal();
    }
    else if (menu == 5){
        load(3);
        system("cls");
        printf("\n\n\n\t\t\t^o^ Terima Kasih ^o^\n\n\n");
        exit(0);
    }
    else{
        printf("Menu yang Anda pilih tidak tersedia!\nSilahkan pilih ulang menu!");
        menu_awal();
    }
}

void menu_utama(){
    system("cls");
    struct today waktu;
    int menu;
    printf("------------------------------------------\n");
    printf(">>              MENU UTAMA              <<\n");
    printf("------------------------------------------\n");
    header(waktu);
    printf("[1] Masuk sebagai pengunjung\n");
    printf("[2] Masuk sebagai petugas\n");
    printf("[3] Peraturan Perpustakaan\n");
    printf("[4] Kembali ke Menu Awal\n");
    printf("Pilih Menu : ");
    scanf("%d", &menu);
    if (menu == 1){
        menu_anggota();
        menu_utama();
    }
    else if (menu == 2){
        login_admin();
        menu_admin();
        menu_utama();
    }
    else if (menu == 3){
        isifiletxt("peraturan.txt");
        menu_utama();
    }
    else if (menu == 4){
        printf("Anda akan diarahkan ke Menu Awal\n");
        load(3);
    }
    else{
        printf("Menu yang Anda pilih tidak tersedia!\nSilahkan pilih ulang menu!");
        menu_utama();
    } 
}

void menu_admin(){
    system("cls");
    int menu;
    struct today waktu;
    printf("------------------------------------------\n");
    printf(">>              MENU ADMIN              <<\n");
    printf("------------------------------------------\n");
    header(waktu);
    printf("[1] Database Anggota\n");
    printf("[2] Database Buku\n");
    printf("[3] Peminjaman dan Pengembalian Buku\n");
    printf("[4] Log Out\n");
    printf("Pilih Menu : ");
    scanf("%d", &menu);
    if (menu == 1){
        database_anggota();
        menu_admin();
    }
    else if (menu == 2){
        database_buku();
        menu_admin();
    }
    else if (menu == 3){
        pinjam_kembali();
        menu_admin();
    }
    else if (menu == 4){
        load(3);
        printf("Log out berhasil\nAnda akan diarahkan ke Menu Utama");
        Sleep(1500);
    }
    else{
        printf("Menu yang Anda pilih tidak tersedia!\nSilahkan pilih ulang menu!");
        menu_admin();
    }
}

void menu_anggota(){
    system("cls");
    int menu, id;
    char judul[40];
    struct today waktu;
    printf("------------------------------------------\n");
    printf(">>             Menu Anggota             <<\n");
    printf("------------------------------------------\n");
    header(waktu);
    //sebenernya make || tpi biar bisa diedit pake && dulu
    if (waktu.jam>16 && waktu.jam<9){
        printf("Maaf Anda tidak bisa mengunjungi perpustakaan\ndi luar jam operasional! (09.00 - 16.00)\nSilahkan kembali lagi besok, terima kasih.");
    }
    else{
        printf("[1] Cari buku berdasarkan Kategori\n");
        printf("[2] Cari buku berdasarkan judul\n");
        printf("[3] Daftar buku\n");
        printf("[4] Lihat riwayat peminjaman\n");
        printf("[4] Kembali ke Menu Utama\n");
        printf("Pilih Menu : ");
        scanf("%d", &menu);
        if (menu == 1){
            system("cls");
            header(waktu);
            printf("Masukan kategori : ");
            scanf("\n");
            scanf("%[^\n]s", judul);
            display_buku(judul, 1);
            menu_anggota();
        }
        else if (menu == 2){
            system("cls");
            header(waktu);
            printf("Masukan judul buku : ");
            scanf("\n");
            scanf("%[^\n]s", judul);
            display_buku(judul, 2);
            menu_anggota();
        }
        else if (menu == 3){
            display_buku("semua", 1);
            menu_anggota();
        }
        else if (menu == 4){
            printf("Masukan ID : ");
            scanf("%d", &id);
            display_anggota("semua", id);
        }
        else if (menu == 5){
            load(3);
            system("cls");
            printf("\n\n\n\t\t\t^o^ Terima Kasih ^o^\n\n\n");
        }
        else{
            printf("Menu yang Anda pilih tidak tersedia!\nSilahkan pilih ulang menu!");
            menu_anggota();
        }
    }
}

void database_anggota(){
    system("cls");
    int menu, id;
    char judul[40];
    struct today waktu;
    printf("------------------------------------------------\n");
    printf(">>              DATABASE ANGGOTA              <<\n");
    printf("-----------------------------------------------\n");
    header(waktu);
    printf("[1] Daftar Anggota\n");
    printf("[2] Tambah Anggota\n");
    printf("[3] Edit Anggota\n");
    printf("[4] Hapus Anggota\n");
    printf("[5] Cari Anggota Berdasarkan ID\n");
    printf("[6] Cari Anggota Berdasarkan Nama\n");
    printf("[7] Kembali ke Menu Admin\n");
    printf("Pilih Menu : ");
    scanf("%d", &menu);
    if (menu == 1){
        display_anggota("judul", 1);
        database_anggota();
    }
    else if (menu == 2){
        tambahanggota();
        database_anggota();
    }
    else if (menu == 3){
        edit_anggota();
        database_anggota();
    }
    else if (menu == 4){
        hapus_anggota();
        database_anggota();
    }
    else if (menu == 5){
        system("cls");
        header(waktu);
        printf("Masukan ID : ");
        scanf("%d", &id);
        display_anggota("judul", id);
        database_anggota();
    }
    else if (menu == 6){
        system("cls");
        header(waktu);
        printf("Masukan nama : ");
        scanf("\n");
        scanf("%[^\n]s", judul);
        display_anggota(judul, 2);
        database_anggota();
    }
    else if (menu == 7){
        load(3);
        printf("Anda akan diarahkan ke Menu Admin");
        Sleep(1500);
    }
    else{
        printf("Menu yang Anda pilih tidak tersedia!\nSilahkan pilih ulang menu!");
        menu_admin();
    }
}

void database_buku(){
    system("cls");
    int menu;
    char judul[40];
    struct today waktu;
    printf("---------------------------------------------\n");
    printf(">>              DATABASE BUKU              <<\n");
    printf("---------------------------------------------\n");
    header(waktu);
    printf("[1] Daftar Buku\n");
    printf("[2] Tambah Buku\n");
    printf("[3] Edit Buku\n");
    printf("[4] Hapus Buku\n");
    printf("[5] Cari Buku Berdasarkan Kategori\n");
    printf("[6] Cari Buku Berdasarkan Judul\n");
    printf("[7] Kembali ke Menu Admin\n");
    printf("Pilih Menu : ");
    scanf("%d", &menu);
    if (menu == 1){
        display_buku(judul, 123);
        database_buku();
    }
    else if (menu == 2){
        tambahbuku();
        database_buku();
    }
    else if (menu == 3){
        edit_buku();
        database_buku();
    }
    else if (menu == 4){
        hapus_buku();
        database_buku();
    }
    else if (menu == 5){
        system("cls");
        header(waktu);
        printf("Masukan kategori : ");
        scanf("\n");
        scanf("%[^\n]s", judul);
        display_buku(judul, 1);
        database_buku();
    }
    else if (menu == 6){
        system("cls");
        header(waktu);
        printf("Masukan judul buku : ");
        scanf("\n");
        scanf("%[^\n]s", judul);
        display_buku(judul, 2);
        database_buku();
    }
    else if (menu == 7){
        load(3);
        printf("Anda akan diarahkan ke Menu Admin");
        Sleep(1500);
    }
    else{
        printf("Menu yang Anda pilih tidak tersedia!\nSilahkan pilih ulang menu!");
        menu_admin();
    }
}

void pinjam_kembali(){
    int id, menu;
    system("cls");
    header(waktu);
    printf("(masukan -1 untuk kembali ke menu admin)\n");
    printf("Masukan ID anggota : ");
    scanf("%d", &id);
    if(id==-1){
        menu_admin();
    }else{
        system("cls");
        header(waktu);
        if(cekanggota("ID", id)){
            printf("ID: %d\n", id);
            printf("[1] Pinjam Buku\n");
            printf("[2] Kembalikan Buku\n");
            printf("[3] Kembali ke Menu Admin\n");
            printf("Pilih Menu : ");
            scanf("%d", &menu);
            switch(menu){
                case 1 :
                    pinjam(id); 
                    pinjam_kembali();
                    break;
                case 2 :
                    kembali(id);
                    pinjam_kembali();
                    break;
                case 3 :
                    break;
                default :
                    printf("Menu yang Anda pilih tidak tersedia\n");
                    system("pause");
                    pinjam_kembali();
            }
        }else{
            printf("ID yang Anda masukan tidak terdaftar!\n");
            pinjam_kembali();
        }
    }
}

int main(){
    menu_awal();
    return 0;
}