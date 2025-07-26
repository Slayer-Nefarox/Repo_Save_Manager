#include <windows.h>
#include <stdio.h>

// Função que copia recursivamente o conteúdo de uma pasta para outra
void CopyFolder(const char* source, const char* destination) 
{
    WIN32_FIND_DATA findData;
    char searchPath[MAX_PATH];
    char srcPath[MAX_PATH];
    char destPath[MAX_PATH];

    // Monta o caminho de busca com curinga para encontrar todos os arquivos/pastas
    snprintf(searchPath, MAX_PATH, "%s\\*", source);
    HANDLE hFind = FindFirstFile(searchPath, &findData); // Inicia a busca de arquivos

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        printf("Folder not found.\n");
        system("pause");
        return; // Se não encontrou nada, sai
    }
    CreateDirectory(destination, NULL); // Cria a pasta de destino (se ainda não existir)

    do {
        // Ignora as entradas "." e ".." (representam diretórios atual e pai)
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;

        // Monta o caminho completo do arquivo/pasta de origem e destino
        snprintf(srcPath, MAX_PATH, "%s\\%s", source, findData.cFileName);
        snprintf(destPath, MAX_PATH, "%s\\%s", destination, findData.cFileName);

        // Se for uma pasta, chama a função recursivamente
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CopyFolder(srcPath, destPath);
        } else {
            // Se for arquivo, copia diretamente
            CopyFile(srcPath, destPath, FALSE);
        }
    } while (FindNextFile(hFind, &findData)); // Continua buscando próximos arquivos

    FindClose(hFind); // Fecha o handle da busca
}

// Função que deleta recursivamente uma pasta e todo seu conteúdo
void DeleteFolder(const char* folderPath) {
    WIN32_FIND_DATA findData;
    char searchPath[MAX_PATH];
    char filePath[MAX_PATH];

    // Monta o caminho para busca com curinga
    snprintf(searchPath, MAX_PATH, "%s\\*", folderPath);
    HANDLE hFind = FindFirstFile(searchPath, &findData);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Folder not found.\n");
        system("pause");
        return; // Se não encontrou nada, sai
    }
    do {
        // Ignora "." e ".."
        if (strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
            continue;

        // Monta o caminho completo do arquivo/pasta atual
        snprintf(filePath, MAX_PATH, "%s\\%s", folderPath, findData.cFileName);

        // Se for pasta, deleta seu conteúdo recursivamente
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            DeleteFolder(filePath);
            RemoveDirectory(filePath); // Remove a pasta após esvaziá-la
        } else {
            DeleteFile(filePath); // Deleta o arquivo
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind); // Fecha o handle
    RemoveDirectory(folderPath); // Por fim, remove a própria pasta
}

int main() {
    char username[256];
    char sourcePath[MAX_PATH];
    char destPath[MAX_PATH];

    // Mensagem de crédito do autor
    printf("Coded by @Slayer_Nefarox \nPlease report bugs on my github \n");
    system("pause"); // Pausa a execução até o usuário pressionar uma tecla

    // Obtém o nome do usuário logado no sistema
    DWORD len = GetEnvironmentVariable("USERNAME", username, sizeof(username));
    int dumb_counter = 0; // Contador de tentativas inválidas

    while (1) {
        // Se o usuário errar muitas vezes, exibe mensagem e encerra
        if (dumb_counter > 3) {
            system("cls"); // Limpa a tela
            for (int i = 0; i < 20; ++i) {
                printf("CRITICAL ERROR: USER MIGHT NOT HAVE INTELLIGENCE ENOUGH TO USE A COMPUTER.\n PLEASE CONTACT A INTELLIGENT ENOUGH ADULT.\n\n");
            }
            system("pause");
            return 0;
        }

        // Menu de opções
        printf("1 - Backup Saves\n2 - Restore Saves\n");
        int op;
        scanf("%d", &op); // Lê a opção do usuário

        // Verifica se conseguiu obter o nome do usuário
        if (len == 0 || len > sizeof(username)) {
            printf("Erro ao obter nome do usuário.\n");
            system("pause");
            return 1;
        }

        // Monta os caminhos das pastas de origem e backup com base no nome de usuário
        snprintf(sourcePath, MAX_PATH, "C:\\Users\\%s\\AppData\\LocalLow\\semiwork\\Repo\\saves", username);
        snprintf(destPath,  MAX_PATH, "C:\\Users\\%s\\AppData\\LocalLow\\semiwork\\Repo\\saves_bkp", username);

        if (op == 1) {
            // Verifica se a pasta "saves" existe
            DWORD attrs = GetFileAttributes(sourcePath);
            if (attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_DIRECTORY)) {
                printf("folder 'saves' not existant.\n");
                system("pause");
                continue;
            }

            // Inicia o backup
            printf("copiying from:\n%s\nto:\n%s\n", sourcePath, destPath);
            CopyFolder(sourcePath, destPath);
            printf("Backup concluded.\n");
            system("pause");
        } else if (op == 2) 
        {
            // Confirmação de restauração (irá apagar o conteúdo atual)
            printf("This will erase the 'saves' folder and restore it to the last backup.\nARE YOU SURE?\n1 - YES\n2 - NO\n");
            scanf("%d", &op);

            if (op == 1) 
            {
                // Verifica se existe o backup
                DWORD bkpAttrs = GetFileAttributes(destPath);
                if (bkpAttrs == INVALID_FILE_ATTRIBUTES || !(bkpAttrs & FILE_ATTRIBUTE_DIRECTORY)) 
                {
                    printf("Backup folder not found. Nothing to restore.\n");
                    system("pause");
                    continue;
                }

                // Se a pasta original existir, deleta
                DWORD savesAttrs = GetFileAttributes(sourcePath);
                if (savesAttrs != INVALID_FILE_ATTRIBUTES && (savesAttrs & FILE_ATTRIBUTE_DIRECTORY)) 
                {
                    printf("Deleting original 'saves' folder...\n");
                    DeleteFolder(sourcePath);
                }

                // Restaura o backup
                printf("Restoring from backup...\n");
                CopyFolder(destPath, sourcePath);
                printf("Restore concluded.\n");
                system("pause");
            } else if (op == 2) 
            {
                // Operação cancelada pelo usuário
                printf("Operation cancelled.\n");
                system("pause");
            } else 
            {
                // Entrada inválida
                printf("ERROR: not a valid operation.\nPlease Try Again.\n");
                dumb_counter++;
                system("pause");
            }
        } else 
        {
            // Entrada inválida
            printf("ERROR: not a valid operation.\nPlease Try Again.\n");
            dumb_counter++;
            system("pause");
        }

        system("cls"); // Limpa tela antes do próximo loop
    }
}
