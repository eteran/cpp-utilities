/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2015 Evan Teran
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <cpp-utilities/uuid.h>
#include <string>

int main() {

	auto ns   = uuid::from_string("{6ba7b810-9dad-11d1-80b4-00c04fd430c8}");
	auto v3 = uuid::v3(ns, "test");
		
	assert(v3.to_string() == "45a113ac-c7f2-30b0-90a5-a399ab912716");
	
	auto v5 = uuid::v5(ns, "test");
	assert(v5.to_string() == "4be0643f-1d98-573b-97cd-ca98a65347dd");

	auto uuid1 = uuid::v4();
	
	assert(!uuid1.is_null());
	assert(uuid1.is_valid());
	assert(uuid1.version() == 4);
	
	assert(uuid::is_valid(uuid1.to_string()));
	
	uuid uuid2;
	assert(uuid2.is_null());
	assert(!uuid2.is_valid());
}
