#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>
//#include <filesystem> c++ 17
//#include <boost/filesystem.hpp>
#include <iomanip> // Para std::setw e std::setfill
#include "../code/graph.hpp"

void save_tree(std::string myText, std::string fileName){
  std::ofstream output("no_commit/arvores/" + fileName, std::ofstream::out | std::ofstream::app);
  output << myText;
  output.close();
}

std::string tree_to_string(Graph &tree){
    std::string value = "";
    int node1, node2;
    for (node1 = 0; node1 < tree.get_num_vertices(); node1++){
        for (auto node2: tree.adjList(node1)){
            if (node2 > node1) continue;
            value = value + "(" + std::to_string(node1) + "," +  std::to_string(node2) + ") ";
        }
    }
    value = value + "\n";
    return value;
}

std::string encontrarArquivoInexistente(const std::string& prefixo, const std::string& extensao, int n) {
    for (int i = 1; i <= n; ++i) {
        std::stringstream nomeArquivo;
        //nomeArquivo << prefixo << '_' << std::setw(2) << std::setfill('0') << i << extensao;
        nomeArquivo << prefixo << '-' << i << extensao;

        std::ifstream arquivo(nomeArquivo.str());
        if (!arquivo.is_open()) {
            return nomeArquivo.str();
        }
    }

    return ""; // Retorna uma string vazia se todos os arquivos existirem
}

bool criarDiretorio(const char* caminho) {
    // Verifica se o diretório existe
    struct stat info;
    if (stat(caminho, &info) != 0) {
        // Se não existe, tenta criá-lo
        if (mkdir(caminho, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
            std::cerr << "Erro ao criar o diretório." << std::endl;
            return false;
        }
    }

    return true;
}

std::string corrigirDiretorio(const std::string& diretorio) {
    // Obtém o caminho absoluto do diretório fornecido
    std::string caminhoAbsoluto = diretorio;

    // Adiciona uma barra no final se não houver
    if (!caminhoAbsoluto.empty() && caminhoAbsoluto.back() != '/') {
        caminhoAbsoluto += '/';
    }

    // Cria o diretório se não existir
    if (!criarDiretorio(caminhoAbsoluto.c_str())) {
        return "";
    }

    return caminhoAbsoluto;
}

// Usando boost/filesystem.hpp
// std::string corrigirDiretorio(const std::string& diretorio) {
//     namespace fs = boost::filesystem;
//     // Obtém o caminho absoluto do diretório fornecido
//     fs::path caminhoAbsoluto = fs::absolute(diretorio);

//     // Adiciona uma barra no final se não houver
//     if (!caminhoAbsoluto.has_filename()) {
//         caminhoAbsoluto /= "";
//     }

//     // Verifica se o diretório existe
//     if (!fs::exists(caminhoAbsoluto)) {
//         // Cria o diretório se não existir
//         if (!fs::create_directory(caminhoAbsoluto)) {
//             std::cerr << "Erro ao criar o diretório." << std::endl;
//             return "";
//         }
//     }

//     return caminhoAbsoluto.string();
// }

// C++ 17
// std::string corrigirDiretorio(const std::string& diretorio) {
//     namespace fs = std::filesystem;
//     // Obtém o caminho absoluto do diretório fornecido
//     fs::path caminhoAbsoluto = fs::absolute(diretorio);

//     // Adiciona uma barra no final se não houver
//     if (!caminhoAbsoluto.has_filename()) {
//         caminhoAbsoluto /= "";
//     }

//     // Verifica se o diretório existe
//     if (!fs::exists(caminhoAbsoluto)) {
//         // Cria o diretório se não existir
//         if (!fs::create_directory(caminhoAbsoluto)) {
//             std::cerr << "Erro ao criar o diretório." << std::endl;
//             return "";
//         }
//     }

//     return caminhoAbsoluto.string();
// }

// int main() {
//     std::string extensaoArquivo = ".txt"; // Substitua pela extensão desejada
//     std::string nomeNaoExistente = encontrarArquivoInexistente("g_XX_XX_XX", extensaoArquivo, N);

//     if (!nomeNaoExistente.empty()) {
//         std::cout << "O arquivo que não existe é: " << nomeNaoExistente << std::endl;
//     } else {
//         std::cout << "Todos os arquivos existem." << std::endl;
//     }

//     return 0;
// }
