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
    // No para atribuicao
    class AssignNode : public ASTNode {
        public:
            AssignNode() : ASTNode(NodeType::ASSIGN) {}

            void set_assignment(std::shared_ptr<ASTNode> id, std::shared_ptr<ASTNode> expr);
            std::shared_ptr<ASTNode> get_id() const;
            std::shared_ptr<ASTNode> get_expression() const;

            void print(int indent = 0) const override;
    };

    // No para operados relacionais
    class RelOp : public ASTNode {
        private:
            std::string op;

        public:
            RelOp(std::string& operation) : ASTNode(NodeType::REL_OP), op(operation) {}

            const std::string& get_operator() const;
            void set_operands(std::shared_ptr<compiler::ASTNode> left, std::shared_ptr<compiler::ASTNode> rigth);

            void print(int indent = 0) const override;
    };

    // No de expressao aritmetica
    class AritOp : public ASTNode {
        private:
            std::string op;
        
        public:
            AritOp(std::string& operation) : ASTNode(NodeType::ARIT_OP), op(operation) {}

            const std::string& get_operator() const;
            void set_operands(std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);

            void print(int indent = 0) const override;
    };

    // No de chamada de funcao
    class CallNode : public ASTNode {
        private:
            std::string function_name;

        public:
            CallNode(const std::string& name) 
                : ASTNode(NodeType::CALL), function_name(name) {}

            const std::string& get_name() const;
            void add_argument(std::shared_ptr<ASTNode> arg);
            size_t num_arguments() const;

            void print(int indent = 0) const override;
    };

    // No's das constantes
    class IdNode : public ASTNode {
        private:
            std::string name;

        public:
            IdNode(std::string &id_name) : ASTNode(NodeType::ID), name(id_name) {}
            
            const std::string& get_name() const;

            void print(int indent = 0) const override;
    };

    class IntConstNode : public ASTNode{
        private:
            int value;
        
        public:
            IntConstNode(int val) : ASTNode(NodeType::INT_CONST), value(val) {
                data_type = DataTypeASA::INT;
            }

            int get_value() const;

            void print(int indent = 0) const override;
            
    };

    // No's das constantes
    class FloatConstNode : public ASTNode{
        private:
            float value;
        
        public:
            FloatConstNode(int val) : ASTNode(NodeType::FLOAT_CONST), value(val) {
                data_type = DataTypeASA::FLOAT;
            }

            float get_value() const;

            void print(int indent = 0) const override;
            
    };

    // No's das constantes
    class CharConstNode : public ASTNode{
        private:
            char value;
        
        public:
            CharConstNode(int val) : ASTNode(NodeType::CHAR_CONST), value(val) {
                data_type = DataTypeASA::CHAR;
            }

            char get_value() const;

            void print(int indent = 0) const override;
            
    };
}

#endif