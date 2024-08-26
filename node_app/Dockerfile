
FROM node:14

WORKDIR /usr/src/app

COPY package*.json ./

# Install the dependencies
RUN npm install

COPY . .

CMD ["npm", "start"]
