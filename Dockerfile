FROM alpine:3.17
RUN apk add coreutils iperf3
# RUN apk add iperf3
CMD ["sh"]