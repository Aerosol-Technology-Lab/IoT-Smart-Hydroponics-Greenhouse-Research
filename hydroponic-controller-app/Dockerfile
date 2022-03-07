FROM node:14.19.0

WORKDIR /app

RUN apt-get update

# I think you need to install following 
RUN apt-get -y install libgtkextra-dev libgconf2-dev libnss3 libasound2 libxtst-dev libxss1 libgbm-dev
RUN npm install --save-dev electron

COPY package*.json ./

COPY . .

ENV PORT=3000

EXPOSE 3000

RUN yarn

RUN yarn elc-rebuild

CMD [ "yarn", "electron-dev" ]
