#include <vector>
#include <string>

class FormatVisitor : public BaseVisitor {
 private:
  std::vector<std::string> strings = {""};
  uint32_t cur_pad = 0;
  uint32_t cur_str = 0;

 public:
  void Visit(const BaseNode* node) override {
    node->Visit(this);
  }
  void Visit(const ClassDeclarationNode* node) override {
    strings[cur_str]+=(std::string("class " + node->ClassName() + " {"));
    strings.push_back(std::string("  public:").insert(0, cur_pad, ' '));
    cur_str += 1;
    cur_pad += 4;
    for (auto field : node->PublicFields()) {
      cur_str++;
      strings.push_back(std::string("").insert(0, cur_pad, ' '));
      Visit(field);
      strings[cur_str].push_back(';');
    }
    if (node->ProtectedFields().size() != 0) {
      strings.push_back("");
      strings.push_back("  protected:");
      cur_str += 2;
      for (auto field : node->ProtectedFields()) {
        cur_str++;
        strings.push_back(std::string("").insert(0, cur_pad, ' '));
        Visit(field);
        strings[cur_str].push_back(';');
      }
    }
    if (node->PrivateFields().size() != 0) {
      strings.push_back("");
      strings.push_back("  private:");
      cur_str += 2;
      for (auto field : node->PrivateFields()) {
        cur_str++;
        strings.push_back(std::string("").insert(0, cur_pad, ' '));
        Visit(field);
        strings[cur_str].push_back(';');
      }
    }
    cur_pad -= 4;
    strings.push_back(std::string("}").insert(0, cur_pad, ' '));
    cur_str++;
    if (cur_pad == 0) {
      strings[cur_str].push_back(';');
    }
  };
  void Visit(const VarDeclarationNode* node) override {
    strings[cur_str] += (std::string(node->TypeName() +
        " " + node->VarName()));
  };
  void Visit(const MethodDeclarationNode* node) override {
    strings[cur_str] += std::string(node->ReturnTypeName() +
        " " + node->MethodName() + "(");
    for (auto argument : node->Arguments()) {
      Visit(argument);
      strings[cur_str]+=", ";
    }
    if (node->Arguments().size() != 0) {
      strings[cur_str].pop_back();
      strings[cur_str].pop_back();
    }
    strings[cur_str].push_back(')');
  };
  const std::vector<std::string>& GetFormattedCode() const {
    return strings;
  }
};
