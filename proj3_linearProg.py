from pulp import *
# first line:
t, p, max_brinquedos = map(int, input().split())

# Armazenamento de dados:
brinquedos = []
pacotes_especiais = []
brinquedos_pacotes = []
b_vars = {}
p_vars = {}

lucro_maximo = LpProblem("Maximizar_lucro", LpMaximize)

# Informações sobre brinquedos
for i in range(t):
    lucro, capacidade = map(int, input().split())
    brinquedos.append({'lucro': lucro, 'capacidade': capacidade})
    b_vars[i] = LpVariable(f'x{i}', lowBound=0, upBound=capacidade, cat='Integer')

# Criação de cópia independente de b_vars antes de possivelmente adicionar os pacotes especiais
b_vars_unchanged = {k: v for k, v in b_vars.items()}    
# Ler informações sobre pacotes especiais
for j in range(p):
    b1, b2, b3, lucro_pacote = map(int, input().split())
    pacote = {'brinquedos': [b1, b2, b3], 'lucro': lucro_pacote}
    pacotes_especiais.append(pacote)
    p_vars[j] = LpVariable(f'p{j}', lowBound=0, cat='Integer')
    lucro_maximo += p_vars[j]*3 <= max_brinquedos
    # Adicionar os brinquedos dos pacotes especiais aos brinquedos individuais
    b_vars[b1-1] += p_vars[j]
    b_vars[b2-1] += p_vars[j]
    b_vars[b3-1] += p_vars[j]
    if b1-1 not in brinquedos_pacotes:
        brinquedos_pacotes.append(b1-1)
    if b2-1 not in brinquedos_pacotes:
        brinquedos_pacotes.append(b2-1)
    if b3-1 not in brinquedos_pacotes:
        brinquedos_pacotes.append(b3-1)
        
        
for b in brinquedos_pacotes:
        lucro_maximo += b_vars[b] <= brinquedos[b]['capacidade']
   
#Restrições
# 1-quantidade de cada brinquedo nos pacotes com os individuais tem de ser <= capacidade do brinquedo 
# 2-soma de todas as variáveis é menor ou igual a max_brinquedos

# maybe fazer estes dentro 
lucro_maximo += lpSum(b_vars[i] for i in range(t)) <= max_brinquedos

# Função objetivo
lucro_maximo += lpSum(brinquedos[i]['lucro'] * b_vars_unchanged[i] for i in range(t)) + \
                lpSum(pacotes_especiais[j]['lucro'] * p_vars[j] for j in range(p))

#print(lucro_maximo)

# Solve/optimization
lucro_maximo.solve(GLPK(msg = 0))
  
# Resultado:

print(value(lucro_maximo.objective))

#for var in lucro_maximo.variables():
#    print(var.name,"==>",var.varValue)
    


