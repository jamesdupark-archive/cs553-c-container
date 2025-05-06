#! /bin/bash
IMAGE_PRESENT="$(docker image ls | grep docker-alpine | wc -l)"
echo $IMAGE_PRESENT
if [ $IMAGE_PRESENT -ne 1 ];
then
    echo "building image..."
    docker build --tag 'docker-alpine' .
fi

docker run -it 'docker-alpine'


# docker container prune
# docker rmi docker-alpine