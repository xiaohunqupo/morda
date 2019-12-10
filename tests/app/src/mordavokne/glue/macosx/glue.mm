#include "../../application.hpp"

#include <papki/FSFile.hpp>

#include <morda/util/util.hpp>

#include <mordaren/OpenGL2Renderer.hpp>

#import <Cocoa/Cocoa.h>


using namespace mordavokne;


#include "../unixCommon.cxx"
#include "../friendAccessors.cxx"

@interface CocoaView : NSView{
	NSTrackingArea* ta;
}

-(id)initWithFrame:(NSRect)rect;
-(void)dealloc;

-(void)mouseDown: (NSEvent*)e;
-(void)mouseUp: (NSEvent*)e;
-(void)rightMouseDown: (NSEvent*)e;
-(void)rightMouseUp: (NSEvent*)e;
-(void)otherMouseDown: (NSEvent*)e;
-(void)otherMouseUp: (NSEvent*)e;
-(void)scrollWheel:(NSEvent*)e;

-(void)mouseDragged: (NSEvent*)e;
-(void)rightMouseDragged: (NSEvent*)e;
-(void)otherMouseDragged: (NSEvent*)e;
-(void)mouseMoved: (NSEvent*)e;
-(void)mouseEntered: (NSEvent*)e;
-(void)mouseExited: (NSEvent*)e;

-(void)keyDown:(NSEvent*)e;
-(void)keyUp:(NSEvent*)e;

@end

@interface CocoaWindow : NSWindow <NSWindowDelegate>{
	CocoaView* v;
}

-(id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)windowStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferCreation;
-(void)dealloc;

-(BOOL)canBecomeKeyWindow;
-(BOOL)canBecomeMainWindow;
-(BOOL)acceptsFirstResponder;

-(void)windowDidResize:(NSNotification*)n;
-(BOOL)windowShouldClose:(id)sender;
-(NSSize)windowWillResize:(NSWindow*)sender toSize:(NSSize)frameSize;

-(void)initStuff;

@end

namespace{
struct WindowWrapper : public utki::Unique{
	NSApplication* applicationObjectId;
	CocoaWindow* windowObjectId;
	NSOpenGLContext* openglContextId;

	bool quitFlag = false;

	bool mouseCursorIsCurrentlyVisible = true;

	WindowWrapper(const window_params& wp);

	~WindowWrapper()noexcept;
};

WindowWrapper& getImpl(const std::unique_ptr<utki::Unique>& pimpl){
	ASSERT(dynamic_cast<WindowWrapper*>(pimpl.get()))
	return static_cast<WindowWrapper&>(*pimpl);
}

}



namespace{


void mouseButton(NSEvent* e, bool isDown, morda::MouseButton_e button){
	NSPoint winPos = [e locationInWindow];
	auto pos = morda::Vec2r(winPos.x, winPos.y).rounded();
	handleMouseButton(
			mordavokne::application::inst(),
			isDown,
			morda::Vec2r(pos.x, mordavokne::application::inst().window_dimensions().y - pos.y),
			button,
			0
		);
}

void macosx_HandleMouseMove(const morda::Vec2r& pos, unsigned id){
//	TRACE(<< "Macosx_HandleMouseMove(): pos = " << pos << std::endl)
	handleMouseMove(
			mordavokne::application::inst(),
			morda::Vec2r(pos.x, mordavokne::application::inst().window_dimensions().y - pos.y),
			id
		);
}

void macosx_HandleMouseHover(bool isHovered){
	auto& ww = getImpl(getWindowPimpl(mordavokne::application::inst()));
	if(!ww.mouseCursorIsCurrentlyVisible){
		if(isHovered){
			[NSCursor hide];
		}else if(!isHovered){
			[NSCursor unhide];
		}
	}

	handleMouseHover(mordavokne::application::inst(), isHovered, 0);
}

void macosx_HandleKeyEvent(bool isDown, morda::key keyCode){
	auto& ww = getImpl(getWindowPimpl(mordavokne::application::inst()));
	handleKeyEvent(mordavokne::application::inst(), isDown, keyCode);
}

class MacosxUnicodeProvider : public morda::Morda::UnicodeProvider{
	const NSString* nsStr;
public:
	MacosxUnicodeProvider(const NSString* nsStr = nullptr) :
			nsStr(nsStr)
	{}

	std::u32string get()const override{
		if(!this->nsStr){
			return std::u32string();
		}

		NSUInteger len = [this->nsStr length];

		std::u32string ret(len, 0);
		for(unsigned i = 0; i != len; ++i){
			ret[i] = [this->nsStr characterAtIndex:i];
		}

		return ret;
	}
};

void macosx_HandleCharacterInput(const void* nsstring, morda::key key){
	auto& ww = getImpl(getWindowPimpl(mordavokne::application::inst()));
	handleCharacterInput(mordavokne::application::inst(), MacosxUnicodeProvider(reinterpret_cast<const NSString*>(nsstring)), key);
}

void macosx_UpdateWindowRect(const morda::Rectr& r){
	auto& ww = getImpl(getWindowPimpl(mordavokne::application::inst()));
	[ww.openglContextId update];//after resizing window we need to update OpenGL context
	updateWindowRect(mordavokne::application::inst(), r);
}

const std::array<morda::key, std::uint8_t(-1) + 1> keyCodeMap = {{
	morda::key::A, //0
	morda::key::S,
	morda::key::D,
	morda::key::F,
	morda::key::H,
	morda::key::G, //5
	morda::key::Z,
	morda::key::X,
	morda::key::C,
	morda::key::V,
	morda::key::UNKNOWN, //0x0A
	morda::key::B,
	morda::key::Q,
	morda::key::W,
	morda::key::E,
	morda::key::R, //15
	morda::key::Y,
	morda::key::T,
	morda::key::ONE,
	morda::key::TWO,
	morda::key::THREE, //20
	morda::key::FOUR,
	morda::key::SIX,
	morda::key::FIVE, //0x17
	morda::key::EQUALS,
	morda::key::NINE, //25
	morda::key::SEVEN,
	morda::key::MINUS,
	morda::key::EIGHT,
	morda::key::ZERO,
	morda::key::RIGHT_SQUARE_BRACKET, //30
	morda::key::O,
	morda::key::U,
	morda::key::LEFT_SQUARE_BRACKET,
	morda::key::I,
	morda::key::P, //35
	morda::key::ENTER, //0x24
	morda::key::L,
	morda::key::J,
	morda::key::APOSTROPHE,
	morda::key::K, //40
	morda::key::SEMICOLON,
	morda::key::BACKSLASH,
	morda::key::COMMA,
	morda::key::SLASH,
	morda::key::N, //0x2D, 45
	morda::key::M,
	morda::key::PERIOD,
	morda::key::TAB, //0x30
	morda::key::SPACE, //0x31
	morda::key::GRAVE, //50
	morda::key::BACKSPACE, //0x33
	morda::key::UNKNOWN, //0x34
	morda::key::ESCAPE, //0x35
	morda::key::UNKNOWN, //0x36
	morda::key::WINDOWS, //Command, 0x37, 55
	morda::key::LEFT_SHIFT, //0x38
	morda::key::CAPSLOCK, //0x39
	morda::key::UNKNOWN, //Option, 0x3A
	morda::key::LEFT_CONTROL, //0x3B
	morda::key::RIGHT_SHIFT, //0x3C, 60
	morda::key::UNKNOWN, //RightOption, 0x3D
	morda::key::RIGHT_CONTROL, //0x3E
	morda::key::FUNCTION, //0x3F
	morda::key::F17, //0x40
	morda::key::UNKNOWN, //KeypadDecimal, 0x41, 65
	morda::key::UNKNOWN, //0x42
	morda::key::UNKNOWN, //KeypadMultiplym 0x43
	morda::key::UNKNOWN, //0x44
	morda::key::UNKNOWN, //KeypadPlus, 0x45
	morda::key::UNKNOWN, //0x46, 70
	morda::key::UNKNOWN, //KeypadClear, 0x47
	morda::key::UNKNOWN, //VolumeUp, 0x48
	morda::key::UNKNOWN, //VolumeDown, 0x49
	morda::key::UNKNOWN, //Mute, 0x4A
	morda::key::UNKNOWN, //KeypadDivide, 0x4B, 75
	morda::key::UNKNOWN, //KeypadEnter, 0x4C
	morda::key::UNKNOWN, //0x4D
	morda::key::UNKNOWN, //KeypadMinus
	morda::key::F18, //0x4F
	morda::key::F19, //0x50, 80
	morda::key::UNKNOWN, //KeypadEquals, 0x51
	morda::key::UNKNOWN, //Keypad0
	morda::key::UNKNOWN, //Keypad1
	morda::key::UNKNOWN, //Keypad2
	morda::key::UNKNOWN, //Keypad3, 85
	morda::key::UNKNOWN, //Keypad4
	morda::key::UNKNOWN, //Keypad5
	morda::key::UNKNOWN, //Keypad6
	morda::key::UNKNOWN, //Keypad7, 0x59
	morda::key::F20, //0x5A, 90
	morda::key::UNKNOWN, //Keypad8, 0x5B
	morda::key::UNKNOWN, //Keypad9, 0x5C
	morda::key::UNKNOWN, //0x5D
	morda::key::UNKNOWN, //0x5E
	morda::key::UNKNOWN, //0x5F, 95
	morda::key::F5, //0x60
	morda::key::F6, //0x61
	morda::key::F7, //0x62
	morda::key::F3, //0x63
	morda::key::F8, //0x64, 100
	morda::key::F9, //0x65
	morda::key::UNKNOWN, //0x66
	morda::key::F11, //0x67
	morda::key::UNKNOWN, //0x68
	morda::key::F13, //0x69
	morda::key::F16, //0x6A
	morda::key::F14, //0x6B
	morda::key::UNKNOWN, //0x6C
	morda::key::F10, //0x6D
	morda::key::UNKNOWN, //0x6E
	morda::key::F12, //0x6F
	morda::key::UNKNOWN, //0x70
	morda::key::F15, //0x71
	morda::key::UNKNOWN, //Help, 0x72
	morda::key::HOME, //0x73
	morda::key::PAGE_UP, //0x74
	morda::key::DELETE, //0x75
	morda::key::F4, //0x76
	morda::key::END, //0x77
	morda::key::F2, //0x78
	morda::key::PAGE_DOWN, //0x79
	morda::key::F1, //0x7A
	morda::key::LEFT, //0x7B
	morda::key::RIGHT, //0x7C
	morda::key::DOWN, //0x7D
	morda::key::UP, //0x7E
	morda::key::UNKNOWN, //0x7F
	morda::key::UNKNOWN, //0x80
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0x90
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0xA0
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0xB0
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0xC0
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0xD0
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0xE0
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN, //0xF0
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN,
	morda::key::UNKNOWN //0xFF
}};

}


@implementation CocoaView

-(id)initWithFrame:(NSRect)rect{
	self = [super initWithFrame:rect];
	if(!self){
		return nil;
	}
	self->ta = [[NSTrackingArea alloc]
			initWithRect: rect
			options: (NSTrackingActiveAlways | NSTrackingInVisibleRect | NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved)
			owner: self
			userInfo: nil
		];
	[self addTrackingArea:self->ta];
	return self;
}

-(void)dealloc{
	[self->ta release];
	[super dealloc];
}

-(void)mouseDown: (NSEvent*)e{
//	TRACE(<< "left down!!!!!" << std::endl)
	mouseButton(e, true, morda::MouseButton_e::LEFT);
}

-(void)mouseUp: (NSEvent*)e{
//	TRACE(<< "left up!!!!!" << std::endl)
	mouseButton(e, false, morda::MouseButton_e::LEFT);
}

-(void)rightMouseDown: (NSEvent*)e{
//	TRACE(<< "right down!!!!!" << std::endl)
	mouseButton(e, true, morda::MouseButton_e::RIGHT);
}

-(void)rightMouseUp: (NSEvent*)e{
//	TRACE(<< "right up!!!!!" << std::endl)
	mouseButton(e, false, morda::MouseButton_e::RIGHT);
}

-(void)otherMouseDown: (NSEvent*)e{
//	TRACE(<< "middle down!!!!!" << std::endl)
	mouseButton(e, true, morda::MouseButton_e::MIDDLE);
}

-(void)otherMouseUp: (NSEvent*)e{
//	TRACE(<< "middle up!!!!!" << std::endl)
	mouseButton(e, false, morda::MouseButton_e::MIDDLE);
}

-(void)scrollWheel: (NSEvent*)e{
//	TRACE(<< "mouse wheel!!!!!" << std::endl)

	if([e hasPreciseScrollingDeltas] == NO){
		morda::MouseButton_e button;
//		TRACE(<< "dy = " << float(dy) << std::endl)
		if([e scrollingDeltaY] < 0){
			button = morda::MouseButton_e::WHEEL_DOWN;
		}else{
			button = morda::MouseButton_e::WHEEL_UP;
		}
//		TRACE(<< "button = " << unsigned(button) << std::endl)

		mouseButton(e, true, button);
		mouseButton(e, false, button);
	}else{
		TRACE(<< "mouse wheel: precise scrolling deltas, UNIMPLEMENTED!!!!!" << std::endl)
	}
}

-(void)mouseMoved: (NSEvent*)e{
//	TRACE(<< "mouseMoved event!!!!!" << std::endl)
	NSPoint pos = [e locationInWindow];
//	TRACE(<< "x = " << pos.x << std::endl)
	macosx_HandleMouseMove(
			morda::Vec2r(pos.x, pos.y).rounded(),
			0
		);
}

-(void)mouseDragged: (NSEvent*)e{
	[self mouseMoved:e];
}

-(void)rightMouseDragged: (NSEvent*)e{
	[self mouseMoved:e];
}

-(void)otherMouseDragged: (NSEvent*)e{
	[self mouseMoved:e];
}

-(void)mouseEntered: (NSEvent*)e{
//	TRACE(<< "mouseEntered event!!!!!" << std::endl)
	[[self window] setAcceptsMouseMovedEvents:YES];
	macosx_HandleMouseHover(true);
}

-(void)mouseExited: (NSEvent*)e{
//	TRACE(<< "mouseExited event!!!!!" << std::endl)
	[[self window] setAcceptsMouseMovedEvents:NO];
	macosx_HandleMouseHover(false);
}

-(void)keyDown:(NSEvent*)e{
//	TRACE(<< "keyDown event!!!!!" << std::endl)
	std::uint8_t kc = [e keyCode];
	morda::key key = keyCodeMap[kc];

	if([e isARepeat] == YES){
		macosx_HandleCharacterInput([e characters], key);
		return;
	}

	macosx_HandleKeyEvent(true, key);

	macosx_HandleCharacterInput([e characters], key);
}

-(void)keyUp:(NSEvent*)e{
//	TRACE(<< "keyUp event!!!!!" << std::endl)
	std::uint8_t kc = [e keyCode];
	macosx_HandleKeyEvent(false, keyCodeMap[kc]);
}

@end



@implementation CocoaWindow

-(id)initWithContentRect:(NSRect)contentRect styleMask:(NSUInteger)windowStyle backing:(NSBackingStoreType)bufferingType defer:(BOOL)deferCreation{
	self = [super initWithContentRect:contentRect styleMask:windowStyle backing:bufferingType defer:deferCreation];
	if(!self){
		return nil;
	}
//	[self setLevel:NSFloatingWindowLevel];
	[self setLevel:NSNormalWindowLevel];

	self->v = [[CocoaView alloc] initWithFrame:[self frameRectForContentRect:contentRect]];
	[self setContentView:self->v];

	[self initStuff];

	[self setShowsResizeIndicator:YES];
	[self setMinSize:NSMakeSize(0, 0)];
	[self setMaxSize:NSMakeSize(1000000000, 1000000000)];
	[self setIgnoresMouseEvents:NO];

	return self;
}

-(void)initStuff{
	[self makeFirstResponder:self->v];
	[self setDelegate:self];
	[self makeKeyWindow];
	[self makeMainWindow];
}

-(void)dealloc{
	[self->v release];
	[super dealloc];
}

-(void)windowDidResize:(NSNotification*)n{
	TRACE(<< "window resize!!!!" << std::endl)
	NSWindow* nsw = [n object];
	NSRect frame = [nsw frame];
	NSRect rect = [nsw contentRectForFrameRect:frame];
	macosx_UpdateWindowRect(morda::Rectr(0, 0, rect.size.width, rect.size.height));
}

-(NSSize)windowWillResize:(NSWindow*)sender toSize:(NSSize)frameSize{
	return frameSize;
}

-(BOOL)windowShouldClose:(id)sender{
	TRACE(<< "window wants to close!!!!" << std::endl)
	application::inst().quit();
	return NO;
}

-(BOOL)canBecomeKeyWindow{return YES;} //This is needed for window without title bar to be able to get key events
-(BOOL)canBecomeMainWindow{return YES;}
-(BOOL)acceptsFirstResponder{return YES;}

-(CocoaView*)view{return self->v;}

@end


namespace{


WindowWrapper::WindowWrapper(const window_params& wp){
	TRACE(<< "WindowWrapper::WindowWrapper(): enter" << std::endl)
	this->applicationObjectId = [NSApplication sharedApplication];

	if(!this->applicationObjectId){
		throw morda::Exc("morda::application::ApplicationObject::ApplicationObject(): failed to create application object");
	}

	utki::ScopeExit scopeExitApplication([this](){
		[this->applicationObjectId release];
	});

	this->windowObjectId = [[CocoaWindow alloc]
		initWithContentRect:NSMakeRect(0, 0, wp.dim.x, wp.dim.y)
		styleMask:(NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskClosable | NSWindowStyleMaskTitled)
		backing:NSBackingStoreBuffered
		defer:NO
	];

	if(!this->windowObjectId){
		throw morda::Exc("morda::application::WindowObject::WindowObject(): failed to create Window object");
	}

	utki::ScopeExit scopeExitWindow([this](){
		[this->windowObjectId release];
	});

	[this->windowObjectId setTitle:[[NSProcessInfo processInfo] processName]];

	{
		std::vector<NSOpenGLPixelFormatAttribute> attributes;
		attributes.push_back(NSOpenGLPFAAccelerated);
		attributes.push_back(NSOpenGLPFAColorSize); attributes.push_back(24);
		if(wp.buffers.get(window_params::buffer_type::depth)){
			attributes.push_back(NSOpenGLPFADepthSize); attributes.push_back(16);
		}
		if(wp.buffers.get(window_params::buffer_type::stencil)){
			attributes.push_back(NSOpenGLPFAStencilSize); attributes.push_back(8);
		}
		attributes.push_back(NSOpenGLPFADoubleBuffer);
		attributes.push_back(NSOpenGLPFASupersample);
		attributes.push_back(0);

		NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:&*attributes.begin()];
		if(pixelFormat == nil){
			throw morda::Exc("morda::application::OpenGLContext::OpenGLContext(): failed to create pixel format");
		}

		this->openglContextId = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
		[pixelFormat release];

		if(!this->openglContextId){
			throw morda::Exc("morda::application::OpenGLContext::OpenGLContext(): failed to create OpenGL context");
		}
	}

	utki::ScopeExit scopeExitOpenGLContext([this](){
		[this->openglContextId release];
	});

	[this->openglContextId setView:[this->windowObjectId contentView]];
	[this->openglContextId makeCurrentContext];

	if(glewInit() != GLEW_OK){
		throw morda::Exc("GLEW initialization failed");
	}

	scopeExitOpenGLContext.reset();
	scopeExitWindow.reset();
	scopeExitApplication.reset();

	TRACE(<< "WindowWrapper::WindowWrapper(): exit" << std::endl)
}

WindowWrapper::~WindowWrapper()noexcept{
	[this->openglContextId release];
	[this->windowObjectId release];
	[this->applicationObjectId release];
}

}



void application::quit()noexcept{
	auto& ww = getImpl(this->windowPimpl);
	ww.quitFlag = true;
}





int main (int argc, const char** argv){
	TRACE(<< "main(): enter" << std::endl)
	auto app = createAppUnix(argc, argv);

	TRACE(<< "main(): app created" << std::endl)

	auto& ww = getImpl(getWindowPimpl(*app));

	[ww.applicationObjectId activateIgnoringOtherApps:YES];

	[ww.windowObjectId makeKeyAndOrderFront:nil];

	[ww.windowObjectId orderFrontRegardless];

	//in order to get keyboard events we need to be foreground application
	{
		ProcessSerialNumber psn = {0, kCurrentProcess};
		OSStatus status = TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		if(status != errSecSuccess){
			ASSERT(false)
		}
	}

	do{
		render(mordavokne::inst());

		std::uint32_t millis = mordavokne::inst().gui.update();

		NSEvent *event = [ww.applicationObjectId
				nextEventMatchingMask:NSEventMaskAny
				untilDate:[NSDate dateWithTimeIntervalSinceNow:(double(millis) / 1000.0)]
				inMode:NSDefaultRunLoopMode
				dequeue:YES
			];

		if(!event){
			continue;
		}

		do{
//			TRACE_ALWAYS(<< "Event: type = "<< [event type] << std::endl)
			switch([event type]){
				case NSEventTypeApplicationDefined:
					{
						std::unique_ptr<std::function<void()>> m(reinterpret_cast<std::function<void()>*>([event data1]));
						(*m)();
					}
					break;
				default:
					[ww.applicationObjectId sendEvent:event];
					[ww.applicationObjectId updateWindows];
					break;
			}

			event = [ww.applicationObjectId
					nextEventMatchingMask:NSEventMaskAny
					untilDate:[NSDate distantPast]
					inMode:NSDefaultRunLoopMode
					dequeue:YES
				];
		}while(event && !ww.quitFlag);
	}while(!ww.quitFlag);

	return 0;
}


namespace{

morda::real getDotsPerInch(){
	NSScreen *screen = [NSScreen mainScreen];
	NSDictionary *description = [screen deviceDescription];
	NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
	CGSize displayPhysicalSize = CGDisplayScreenSize(
			[[description objectForKey:@"NSScreenNumber"] unsignedIntValue]
		);

	morda::real value = morda::real(((displayPixelSize.width * 10.0f / displayPhysicalSize.width) +
			(displayPixelSize.height * 10.0f / displayPhysicalSize.height)) / 2.0f);
	value *= 2.54f;
	return value;
}

morda::real getDotsPerPt(){
	NSScreen *screen = [NSScreen mainScreen];
	NSDictionary *description = [screen deviceDescription];
	NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
	CGSize displayPhysicalSize = CGDisplayScreenSize(
			[[description objectForKey:@"NSScreenNumber"] unsignedIntValue]
		);

	r4::vec2ui resolution(displayPixelSize.width, displayPixelSize.height);
	r4::vec2ui screenSizeMm(displayPhysicalSize.width, displayPhysicalSize.height);

	return application::get_pixels_per_dp(resolution, screenSizeMm);
}

}//~namespace


application::application(std::string&& name, const window_params& wp) :
		name(name),
		windowPimpl(utki::makeUnique<WindowWrapper>(wp)),
		gui(
				std::make_shared<mordaren::OpenGL2Renderer>(),
				getDotsPerInch(),
				getDotsPerPt(),
				[this](std::function<void()>&& a){
					auto& ww = getImpl(getWindowPimpl(*this));

					NSEvent* e = [NSEvent
							otherEventWithType: NSEventTypeApplicationDefined
							location: NSMakePoint(0, 0)
							modifierFlags:0
							timestamp:0
							windowNumber:0
							context: nil
							subtype: 0
							data1: reinterpret_cast<NSInteger>(new std::function<void()>(std::move(a)))
							data2: 0
						];

					[ww.applicationObjectId postEvent:e atStart:NO];
				}
			),
		storage_dir(initializeStorageDir(this->name))
{
	TRACE(<< "application::application(): enter" << std::endl)
	this->updateWindowRect(
			morda::Rectr(
					0,
					0,
					float(wp.dim.x),
					float(wp.dim.y)
				)
		);
}

void application::swapFrameBuffers(){
	auto& ww = getImpl(this->windowPimpl);
	[ww.openglContextId flushBuffer];
}



void application::set_fullscreen(bool enable){
	if(enable == this->is_fullscreen()){
		return;
	}

	auto& ww = getImpl(this->windowPimpl);

	if(enable){
		//save old window size
		NSRect rect = [ww.windowObjectId frame];
		this->beforeFullScreenWindowRect.p.x = rect.origin.x;
		this->beforeFullScreenWindowRect.p.y = rect.origin.y;
		this->beforeFullScreenWindowRect.d.x = rect.size.width;
		this->beforeFullScreenWindowRect.d.y = rect.size.height;

		[ww.windowObjectId setStyleMask:([ww.windowObjectId styleMask] & (~(NSWindowStyleMaskTitled | NSWindowStyleMaskResizable)))];

		[ww.windowObjectId setFrame:[[NSScreen mainScreen] frame] display:YES animate:NO];
		[ww.windowObjectId setLevel:NSScreenSaverWindowLevel];
	}else{
		[ww.windowObjectId setStyleMask:([ww.windowObjectId styleMask] | NSWindowStyleMaskTitled | NSWindowStyleMaskResizable)];

		NSRect oldFrame;
		oldFrame.origin.x = this->beforeFullScreenWindowRect.p.x;
		oldFrame.origin.y = this->beforeFullScreenWindowRect.p.y;
		oldFrame.size.width = this->beforeFullScreenWindowRect.d.x;
		oldFrame.size.height = this->beforeFullScreenWindowRect.d.y;

		[ww.windowObjectId setFrame:oldFrame display:YES animate:NO];
		[ww.windowObjectId setLevel:NSNormalWindowLevel];
	}

	[ww.windowObjectId initStuff];

	this->isFullscreen_v = enable;
}



void application::set_mouse_cursor_visible(bool visible){
	auto& ww = getImpl(this->windowPimpl);
	if(visible){
		if(!ww.mouseCursorIsCurrentlyVisible){
			[NSCursor unhide];
			ww.mouseCursorIsCurrentlyVisible = true;
		}
	}else{
		if(ww.mouseCursorIsCurrentlyVisible){
			[NSCursor hide];
			ww.mouseCursorIsCurrentlyVisible = false;
		}
	}
}
