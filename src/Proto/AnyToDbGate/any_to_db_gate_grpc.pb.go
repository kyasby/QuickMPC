// Code generated by protoc-gen-go-grpc. DO NOT EDIT.
// versions:
// - protoc-gen-go-grpc v1.2.0
// - protoc             v3.18.0
// source: any_to_db_gate.proto

package AnyToDbGate

import (
	context "context"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
)

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
// Requires gRPC-Go v1.32.0 or later.
const _ = grpc.SupportPackageIsVersion7

// AnyToDbGateClient is the client API for AnyToDbGate service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type AnyToDbGateClient interface {
	// execute query to DbGate container
	ExecuteQuery(ctx context.Context, in *ExecuteQueryRequest, opts ...grpc.CallOption) (AnyToDbGate_ExecuteQueryClient, error)
}

type anyToDbGateClient struct {
	cc grpc.ClientConnInterface
}

func NewAnyToDbGateClient(cc grpc.ClientConnInterface) AnyToDbGateClient {
	return &anyToDbGateClient{cc}
}

func (c *anyToDbGateClient) ExecuteQuery(ctx context.Context, in *ExecuteQueryRequest, opts ...grpc.CallOption) (AnyToDbGate_ExecuteQueryClient, error) {
	stream, err := c.cc.NewStream(ctx, &AnyToDbGate_ServiceDesc.Streams[0], "/anytodbgate.AnyToDbGate/ExecuteQuery", opts...)
	if err != nil {
		return nil, err
	}
	x := &anyToDbGateExecuteQueryClient{stream}
	if err := x.ClientStream.SendMsg(in); err != nil {
		return nil, err
	}
	if err := x.ClientStream.CloseSend(); err != nil {
		return nil, err
	}
	return x, nil
}

type AnyToDbGate_ExecuteQueryClient interface {
	Recv() (*ExecuteQueryResponse, error)
	grpc.ClientStream
}

type anyToDbGateExecuteQueryClient struct {
	grpc.ClientStream
}

func (x *anyToDbGateExecuteQueryClient) Recv() (*ExecuteQueryResponse, error) {
	m := new(ExecuteQueryResponse)
	if err := x.ClientStream.RecvMsg(m); err != nil {
		return nil, err
	}
	return m, nil
}

// AnyToDbGateServer is the server API for AnyToDbGate service.
// All implementations must embed UnimplementedAnyToDbGateServer
// for forward compatibility
type AnyToDbGateServer interface {
	// execute query to DbGate container
	ExecuteQuery(*ExecuteQueryRequest, AnyToDbGate_ExecuteQueryServer) error
	mustEmbedUnimplementedAnyToDbGateServer()
}

// UnimplementedAnyToDbGateServer must be embedded to have forward compatible implementations.
type UnimplementedAnyToDbGateServer struct {
}

func (UnimplementedAnyToDbGateServer) ExecuteQuery(*ExecuteQueryRequest, AnyToDbGate_ExecuteQueryServer) error {
	return status.Errorf(codes.Unimplemented, "method ExecuteQuery not implemented")
}
func (UnimplementedAnyToDbGateServer) mustEmbedUnimplementedAnyToDbGateServer() {}

// UnsafeAnyToDbGateServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to AnyToDbGateServer will
// result in compilation errors.
type UnsafeAnyToDbGateServer interface {
	mustEmbedUnimplementedAnyToDbGateServer()
}

func RegisterAnyToDbGateServer(s grpc.ServiceRegistrar, srv AnyToDbGateServer) {
	s.RegisterService(&AnyToDbGate_ServiceDesc, srv)
}

func _AnyToDbGate_ExecuteQuery_Handler(srv interface{}, stream grpc.ServerStream) error {
	m := new(ExecuteQueryRequest)
	if err := stream.RecvMsg(m); err != nil {
		return err
	}
	return srv.(AnyToDbGateServer).ExecuteQuery(m, &anyToDbGateExecuteQueryServer{stream})
}

type AnyToDbGate_ExecuteQueryServer interface {
	Send(*ExecuteQueryResponse) error
	grpc.ServerStream
}

type anyToDbGateExecuteQueryServer struct {
	grpc.ServerStream
}

func (x *anyToDbGateExecuteQueryServer) Send(m *ExecuteQueryResponse) error {
	return x.ServerStream.SendMsg(m)
}

// AnyToDbGate_ServiceDesc is the grpc.ServiceDesc for AnyToDbGate service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var AnyToDbGate_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "anytodbgate.AnyToDbGate",
	HandlerType: (*AnyToDbGateServer)(nil),
	Methods:     []grpc.MethodDesc{},
	Streams: []grpc.StreamDesc{
		{
			StreamName:    "ExecuteQuery",
			Handler:       _AnyToDbGate_ExecuteQuery_Handler,
			ServerStreams: true,
		},
	},
	Metadata: "any_to_db_gate.proto",
}
