# Imagem oficial do Maven com JDK 17
FROM maven:3.9.6-eclipse-temurin-17

# Diretório de trabalho dentro do container
WORKDIR /app

# Copia o pom.xml e o código-fonte
COPY pom.xml .
COPY src ./src

# Expõe a porta padrão do Spring Boot
EXPOSE 8080

# Comando para rodar o servidor com Maven
CMD ["mvn", "spring-boot:run"]

