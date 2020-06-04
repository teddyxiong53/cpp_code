.PHONY: all clean tcp tcp_async udp udp_async

CXXFLAGS := -std=c++11
LDFLAGS := -lpthread -lboost_system -lboost_thread

define gen_exe
	g++ $(CXXFLAGS)  $(1).cpp -o  $(1) $(LDFLAGS)
endef
default: tcp

all: tcp tcp_async udp udp_async

tcp:
	$(call gen_exe, echo_tcp_server)
	$(call gen_exe, echo_tcp_client)

tcp_async:
	$(call gen_exe, echo_tcp_server_async)
	$(call gen_exe, echo_tcp_client_async)

udp:
	$(call gen_exe, echo_udp_server)
	$(call gen_exe, echo_udp_client)

udp_async:
	$(call gen_exe, echo_udp_server_async)
	$(call gen_exe, echo_udp_client_async)

clean:
	rm -f \
		echo_tcp_server echo_tcp_client \
		echo_tcp_server_async echo_tcp_client_async \
		echo_udp_server echo_udp_client \
		echo_udp_server_async echo_udp_client_async
