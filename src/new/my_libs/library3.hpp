#include <string>
#ifndef LIBRARY3_HPP_ //Se eu não fui escrito anteriormente, então
#define LIBRARY3_HPP_ //Fala para todo mundo no futuro que eu estou escrito agora...
//prossiga escrevendo todo meu conteúdo

void save_tree(std::string myText, std::string fileName);

std::string tree_to_string(Graph &tree);

std::string encontrarArquivoInexistente(const std::string& prefixo, const std::string& extensao, int n);

bool criarDiretorio(const char* caminho);
std::string corrigirDiretorio(const std::string& diretorio);

#endif //LIBRARY3_HPP_ Finaliza o "se" lá em cima