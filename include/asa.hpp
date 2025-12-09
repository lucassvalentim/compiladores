#ifndef ASA_HPP
#define ASA_HPP

#include <vector>
#include <string>
#include <memory>

namespace compiler {
    enum class  NodeType {
        FUNCTION, 
        REL_OP,
        ARIT_OP,
        ASSIGN,
        IF,
        WHILE,
        PRINT,
        RETURN,
        CALL,
        BLOCO,
        ID,
        INT_CONST,
        FLOAT_CONST,
        CHAR_CONST
    };

    enum class DataTypeASA {
        ERROR = -1,
        INT = 0,
        FLOAT,
        CHAR,
        VOID,
        UNDEFINED
    };

    class ASTNode {
        protected:
            NodeType node_type;
            DataTypeASA data_type;
            std::vector<std::shared_ptr<ASTNode>> childrens;

        public:
            ASTNode(NodeType type) : node_type(type), data_type(DataTypeASA::UNDEFINED) {}
            virtual ~ASTNode() = default;
            

            NodeType get_node_type();
            DataTypeASA get_date_type();
            void set_data_type(DataTypeASA type);
            void add_child(std::shared_ptr<ASTNode> child);
            const std::vector<std::shared_ptr<ASTNode>>& get_childrens() const;
            std::shared_ptr<ASTNode> get_child(size_t index) const;

            virtual void print(int indent = 0) const = 0;
        };

    // No raiz - Funcao
    class FunctionNode : public ASTNode {
        private:
            std::string function_name;
        
        public:
            FunctionNode(std::string &name) : ASTNode(NodeType::FUNCTION) , function_name(name) {}

            const std::string& get_function_name () const;
            void set_block(std::shared_ptr<ASTNode> block);

            void print(int indet = 0) const override;
    };

    // No para o bloco de comando
    class BlockNode : public ASTNode {
        public:
            BlockNode() : ASTNode(NodeType::BLOCO) {}

            void add_statement(std::shared_ptr<ASTNode> statement);

            void print(int indent = 0) const override;
    };
}

#endif