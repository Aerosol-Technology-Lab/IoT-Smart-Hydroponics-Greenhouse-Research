FROM node:14.19.0

WORKDIR /app

RUN npm install --global yarn

COPY package*.json ./

COPY . .

ENV PORT=3000

EXPOSE 3000

RUN yarn

RUN yarn elc-rebuild

RUN yarn electron-dev
