#include <stdio.h>
#include <stdlib.h>
 
 /*Implemente um programa que faÃ§a a leitura de uma imagem FAT16 e apresente as seguintes infos:
a) Os arquivos e subdiretorios na raiz
b) Mostre o conteudo de um arquivo escolhido pelo usuario (utilizar o nome no formato 8.3 ou qualquer outra forma
c) Mostre o conteudo de um subdiretorio armazenado no diretorio raiz*/
typedef struct fat_BS{
	unsigned char 		bootjmp[3];
	unsigned char 		oem_name[8];
	unsigned short 	    bytes_per_sector;
	unsigned char		sectors_per_cluster;
	unsigned short		reserved_sector_count;
	unsigned char		table_count;
	unsigned short		root_entry_count;
	unsigned short		total_sectors_16;
	unsigned char		media_type;
	unsigned short		table_size_16;
	unsigned short		sectors_per_track;
	unsigned short		head_side_count;
	unsigned int 		hidden_sector_count;
	unsigned int 		total_sectors_32;

	unsigned char		extended_section[54];
 
}__attribute__((packed)) fat_BS_t;

int main(){

    unsigned int root_init , find_file;
    int position_t, position;
    int Contentposition, Contentposition_t;
    char nome[8], ext[3];

    FILE *fp;
    fat_BS_t  boot_record;

    fp= fopen("fat16_1sectorpercluster.img", "rb");
    fseek(fp, 0, SEEK_SET);
    fread(&boot_record, sizeof(fat_BS_t),1, fp); 

    root_init = boot_record.reserved_sector_count + (boot_record.table_size_16 * boot_record.table_count); //->a 
    find_file =  root_init * boot_record.bytes_per_sector; //buscar os aqruivos dentro do diretorio raiz //->find_file

    printf("Bytes per sector %hd \n", boot_record.bytes_per_sector);
    printf("Sector per cluster %x \n", boot_record.sectors_per_cluster);
	  printf("Table %x \n", boot_record.table_count);
    printf("Inicio do Rootdir = %u\n", root_init);
    printf("Buscar arquvio Rootdir = %u\n", find_file);


    position = find_file;
    position_t = find_file + 11;

    fseek(fp, position, SEEK_SET);
    fread(&Contentposition, 1, 1, fp);

    while(Contentposition != 0){

      fseek(fp, position, SEEK_SET);
      fread(&Contentposition, 1, 1, fp);

      if(Contentposition == 229){
      position_t  += 32;
      position += 32;
      }else{
      fseek(fp, position_t, SEEK_SET);
      fread(&Contentposition_t, 1, 1, fp);

        if(Contentposition_t == 15){
          position_t  += 32;
          position += 32;
        }else if(Contentposition_t == 16){
          printf("DIR\n");
          fseek(fp, position, SEEK_SET);
          fread(&nome, sizeof(char), 8, fp);
          printf("Nome: %s \n", nome);
          printf("---------------------\n");
          position_t  += 32;
          position += 32;
        }

          else if(Contentposition_t == 32){
          printf("FILE\n");
          fseek(fp, position, SEEK_SET);
          fread(&nome, sizeof(char), 8, fp);
          printf("Nome: %s \n", nome);
          
          printf("---------------------\n");
          position_t  += 32;
          position += 32;
        }

         else if(Contentposition_t == 28){
          printf("TAM\n");
          fseek(fp, position, SEEK_SET);
          fread(&nome, sizeof(char), 8, fp);
          printf("Nome: %s \n", nome);
          
          printf("---------------------\n");
          position_t  += 32;
          position += 32;
        }
      }
    }

  return 0;
}

