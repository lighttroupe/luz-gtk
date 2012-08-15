# Copyright 2012 Ian McIntosh

$video_files ||= {}

class ActorEffectVideoFile < ActorEffect
	title				"Video File"
	description ""

	setting 'file_name', :string
	setting 'speed', :float, :range => -4.0..4.0, :default => 1.0..2.0

	def after_load
		require 'video-file/ffmpeg'
		@frame_index = 0
		@fast_forward_time = 0.0
		super
	end

	def tick
		reload_if_needed if $gui
		@fast_forward_time += (speed - 1.0)
		@skip_frames, remainder = @fast_forward_time.divmod(1.0)
		#puts @skip_frames unless @skip_frames == 0.0 
		@fast_forward_time = remainder

		@frame_index += (1 + @skip_frames)
	end

	def render
		return yield unless @file

		@file.with_frame(@frame_index) {
			yield
		}
		@skip_frames = 0
	end

private

	def reload_if_needed
		if file_name != @file_name
			if $engine.project.file_path && File.exist?(path=File.join($engine.project.file_path, file_name))
				if (file = FFmpeg::File.new(path))
					@file = file
				end
			end
			@file_name = file_name
		end
	end
end
