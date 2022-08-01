// Code generated by protoc-gen-go-grpc. DO NOT EDIT.

package ManageToComputationContainer

import (
	context "context"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
	emptypb "google.golang.org/protobuf/types/known/emptypb"
)

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
// Requires gRPC-Go v1.32.0 or later.
const _ = grpc.SupportPackageIsVersion7

// ManageToComputationClient is the client API for ManageToComputation service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://pkg.go.dev/google.golang.org/grpc/?tab=doc#ClientConn.NewStream.
type ManageToComputationClient interface {
	// receive executing computation request from manage container
	ExecuteComputation(ctx context.Context, in *ExecuteComputationRequest, opts ...grpc.CallOption) (*emptypb.Empty, error)
	// receive predict request from manage container
	Predict(ctx context.Context, in *PredictRequest, opts ...grpc.CallOption) (*emptypb.Empty, error)
}

type manageToComputationClient struct {
	cc grpc.ClientConnInterface
}

func NewManageToComputationClient(cc grpc.ClientConnInterface) ManageToComputationClient {
	return &manageToComputationClient{cc}
}

func (c *manageToComputationClient) ExecuteComputation(ctx context.Context, in *ExecuteComputationRequest, opts ...grpc.CallOption) (*emptypb.Empty, error) {
	out := new(emptypb.Empty)
	err := c.cc.Invoke(ctx, "/managetocomputation.ManageToComputation/ExecuteComputation", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

func (c *manageToComputationClient) Predict(ctx context.Context, in *PredictRequest, opts ...grpc.CallOption) (*emptypb.Empty, error) {
	out := new(emptypb.Empty)
	err := c.cc.Invoke(ctx, "/managetocomputation.ManageToComputation/Predict", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// ManageToComputationServer is the server API for ManageToComputation service.
// All implementations must embed UnimplementedManageToComputationServer
// for forward compatibility
type ManageToComputationServer interface {
	// receive executing computation request from manage container
	ExecuteComputation(context.Context, *ExecuteComputationRequest) (*emptypb.Empty, error)
	// receive predict request from manage container
	Predict(context.Context, *PredictRequest) (*emptypb.Empty, error)
	mustEmbedUnimplementedManageToComputationServer()
}

// UnimplementedManageToComputationServer must be embedded to have forward compatible implementations.
type UnimplementedManageToComputationServer struct {
}

func (UnimplementedManageToComputationServer) ExecuteComputation(context.Context, *ExecuteComputationRequest) (*emptypb.Empty, error) {
	return nil, status.Errorf(codes.Unimplemented, "method ExecuteComputation not implemented")
}
func (UnimplementedManageToComputationServer) Predict(context.Context, *PredictRequest) (*emptypb.Empty, error) {
	return nil, status.Errorf(codes.Unimplemented, "method Predict not implemented")
}
func (UnimplementedManageToComputationServer) mustEmbedUnimplementedManageToComputationServer() {}

// UnsafeManageToComputationServer may be embedded to opt out of forward compatibility for this service.
// Use of this interface is not recommended, as added methods to ManageToComputationServer will
// result in compilation errors.
type UnsafeManageToComputationServer interface {
	mustEmbedUnimplementedManageToComputationServer()
}

func RegisterManageToComputationServer(s grpc.ServiceRegistrar, srv ManageToComputationServer) {
	s.RegisterService(&ManageToComputation_ServiceDesc, srv)
}

func _ManageToComputation_ExecuteComputation_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(ExecuteComputationRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(ManageToComputationServer).ExecuteComputation(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/managetocomputation.ManageToComputation/ExecuteComputation",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(ManageToComputationServer).ExecuteComputation(ctx, req.(*ExecuteComputationRequest))
	}
	return interceptor(ctx, in, info, handler)
}

func _ManageToComputation_Predict_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(PredictRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(ManageToComputationServer).Predict(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/managetocomputation.ManageToComputation/Predict",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(ManageToComputationServer).Predict(ctx, req.(*PredictRequest))
	}
	return interceptor(ctx, in, info, handler)
}

// ManageToComputation_ServiceDesc is the grpc.ServiceDesc for ManageToComputation service.
// It's only intended for direct use with grpc.RegisterService,
// and not to be introspected or modified (even as a copy)
var ManageToComputation_ServiceDesc = grpc.ServiceDesc{
	ServiceName: "managetocomputation.ManageToComputation",
	HandlerType: (*ManageToComputationServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "ExecuteComputation",
			Handler:    _ManageToComputation_ExecuteComputation_Handler,
		},
		{
			MethodName: "Predict",
			Handler:    _ManageToComputation_Predict_Handler,
		},
	},
	Streams:  []grpc.StreamDesc{},
	Metadata: "manage_to_computation.proto",
}
